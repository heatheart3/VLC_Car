/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "./include/light.h"
#include "./include/mytimer.h"
#include "./include/myadc.h"
#include "./include/motor.h"
#include "./include/servo.h"
#include "./include/mypid.h"
#include "./include/decoder.h"

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define VLT_HIGH 4095
#define VLT_LOW 0
#define DECODED_MES_LEN 8
#define VALID_MES_LEN 16

#define HEADER_THRES 20
#define DOUBLE_HIGH_THRES 14
#define DOUBLE_LOW_THRES 14
#define SINGLE_HIGH_THRES 5
#define SINGLE_LOW_THRES 5

static char decode_data[DECODED_MES_LEN] = {0};
static char mes_buffer[VALID_MES_LEN + 4] = {0};
static char mes_result[1000] = {0};
static int mes_counter = 0;
static int decect_header_counter = 0;

int overflow_mark = 0;

/*variable for vlc demodulation*/
static u_int16_t tmp_vlt = 0;
static uint8_t low_count = 0;
static uint8_t high_count = 0;
static int bit_counter = -1;
static char mes_start_flg = 0;
/*variable for vlc demodulation*/

enum CAR_STATE current_state = ENUM_STOP;
enum CAR_STATE last_state = ENUM_STOP;

static adc_channel_t channel[1] = {ADC_CHANNEL_7}; // gpio35

extern QueueHandle_t encoder_queue;

void decode(const char *encode, char *data)
{
    int j = 0;
    for (int i = 0; i < DECODED_MES_LEN; i++)
    {
        if (encode[j] == 0)
        {
            if (encode[j + 1] == 0)
            {
                mes_result[mes_counter] = 'E';
                mes_counter++;
                return;
            }
            else
            {
                data[i] = '1';
            }
        }
        else
        {
            if (encode[j + 1] == 1)
            {
                mes_result[mes_counter] = 'E';
                mes_counter++;
                return;
            }
            else
            {
                data[i] = '0';
            }
        }
        j += 2;
    }

    char decode_ascii = 0;
    int tmp_bit = 0;
    for (int i = 0; i < strlen(data); i++)
    {
        tmp_bit = data[i] - '0';
        decode_ascii = decode_ascii | (tmp_bit << (7 - i));
    }
    mes_result[mes_counter] = decode_ascii;
    mes_counter++;
}


static void inline demodulation(const int tmp_vlt)
{
    if (!mes_start_flg)
    {
        if (tmp_vlt >= VLT_HIGH)
        {
            high_count++;
        }
        else if (tmp_vlt <= VLT_LOW)
        {
            if (high_count >= HEADER_THRES)
            {
                mes_start_flg = true;
                high_count = 0;
                // low_count = 0;
                bit_counter = -1;
                decect_header_counter++;
                // printf("mes start!\n");
            }
            else
            {
                high_count = 0;
                low_count = 0;
            }
        }
    }
    else
    {
        if (tmp_vlt >= VLT_HIGH)
        {
            high_count++;
        }
        else if (tmp_vlt <= VLT_LOW)
        {
            low_count++;
        }

        if (high_count != 0 && low_count != 0)
        {
            // dicard the lsb of header [011"0"]
            if (bit_counter == -1)
            {

                bit_counter++;

                if (low_count >= DOUBLE_LOW_THRES)
                {
                    mes_buffer[bit_counter] = 0;
                    bit_counter++;
                }
                low_count = 0;
            }
            else if (bit_counter < VALID_MES_LEN)
            {
                if (high_count > low_count)
                {
                    if (high_count >= DOUBLE_HIGH_THRES)
                    {
                        mes_buffer[bit_counter] = 1;
                        bit_counter++;
                        mes_buffer[bit_counter] = 1;
                        bit_counter++;
                        // printf("11");
                        high_count = 0;
                    }
                    else if (high_count >= SINGLE_HIGH_THRES && high_count < DOUBLE_HIGH_THRES)
                    {
                        mes_buffer[bit_counter] = 1;
                        bit_counter++;
                        // printf("1");
                        high_count = 0;
                    }
                    else
                    {
                        mes_result[mes_counter] = 'e';
                        mes_counter++;
                        mes_start_flg = false;
                        high_count = 0;
                        low_count = 0;
                    }
                }
                else
                {
                    if (low_count >= DOUBLE_LOW_THRES)
                    {
                        mes_buffer[bit_counter] = 0;
                        bit_counter++;
                        mes_buffer[bit_counter] = 0;
                        bit_counter++;
                        // printf("00");
                        low_count = 0;
                    }
                    else if (low_count >= SINGLE_LOW_THRES && low_count < DOUBLE_LOW_THRES)
                    {
                        mes_buffer[bit_counter] = 0;
                        bit_counter++;
                        // printf("0");
                        low_count = 0;
                    }
                    else
                    {
                        mes_result[mes_counter] = 'e';
                        mes_counter++;
                        mes_start_flg = false;
                        low_count = 0;
                        high_count = 0;
                    }
                }
            }

            // OOK Demodulation Done
            // Codes about control car are written below
            if (bit_counter >= VALID_MES_LEN)
            {
                // Decode Message
                decode(mes_buffer, decode_data);
                // car_control(decode_data,24);
                mes_start_flg = false;
                bit_counter = -1;
                memset(mes_buffer, 0, sizeof(mes_buffer));
                memset(decode_data, 0, sizeof(decode_data));
            }
        }
    }
}
void app_main(void)
{

    // motor_config();
    // servo_config();
    // turn_forward();

    // esp_err_t ret;
    // uint32_t ret_num = 0;
    // uint8_t result[EXAMPLE_READ_LEN] = {0};
    // memset(result, 0xcc, EXAMPLE_READ_LEN);

    // adc_continuous_handle_t handle = NULL;
    // adc_continuous_evt_cbs_t adc1_handle;
    // adc1_handle.on_pool_ovf = detect_overflow;
    // continuous_adc_init(channel, sizeof(channel) / sizeof(adc_channel_t), &handle);
    // ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(handle, &adc1_handle, NULL));
    // ESP_ERROR_CHECK(adc_continuous_start(handle));

    // memset(mes_buffer, 0, sizeof(mes_buffer));
    // memset(decode_data, 0, sizeof(decode_data));
    printf("Start demo!\n");
    uint8_t symbols[]={34,1,25,18,60,44,42,62,52,39,24,48,57,1,43,25,54,62,53,25,45,14,4,50,13,20,24,28,1,32,44,63,6};
    uint8_t spinal_mes[4]={0};
    while (1)
    {
        printf("Start to printf Message\n");
        SpinalDecode(symbols,spinal_mes);
        for(int i=0;i<4;i++){
            printf("%c",spinal_mes[i]);
        }
        printf("\n");
        printf("Decode Done!\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // // log the demoluation result
        // if (mes_counter >= 1000)
        // {
        //     ESP_ERROR_CHECK(adc_continuous_stop(handle));
        //     // printf("Start to printf Message\n");
        //     // printf("Totally dectect %d messages\n", decect_header_counter);
        //     bit_counter = -1;
        //     mes_start_flg = false;
        //     high_count = 0;
        //     low_count = 0;
        //     memset(mes_buffer, 0, sizeof(mes_buffer));
        //     memset(decode_data, 0, sizeof(decode_data));
        //     for (int i = 0; i < 100; i++)
        //     {
        //         printf("%c\n", mes_result[i]);
        //     }
        //     mes_counter = 0;
        //     ESP_ERROR_CHECK(adc_continuous_start(handle));
        // }

        // ret = adc_continuous_read(handle, result, EXAMPLE_READ_LEN, &ret_num, 0);

        // // detect the overflow
        // if (overflow_mark == 1)
        // {
        //     printf("overflow\n");
        //     bit_counter = -1;
        //     mes_start_flg = false;
        //     high_count = 0;
        //     low_count = 0;
        //     memset(mes_buffer, 0, sizeof(mes_buffer));
        //     memset(decode_data, 0, sizeof(decode_data));
        //     overflow_mark = 0;
        // }

        // // if the adc read is successful
        // if (ret == ESP_OK)
        // {

        //     // ESP_LOGI("TASK", "ret is %x, ret_num is %" PRIu32, ret, ret_num);?
        //     ESP_ERROR_CHECK(adc_continuous_stop(handle));

        //     int adc_inverse = 0;
        //     int tmp_adc[2] = {0};
        //     for (int i = 0; i < ret_num; i += SOC_ADC_DIGI_RESULT_BYTES)
        //     {
        //         adc_digi_output_data_t *p = (void *)&result[i];
        //         if (check_valid_data(p))
        //         {
        //             tmp_adc[adc_inverse] = p->type1.data;
        //             adc_inverse++;
        //             if (adc_inverse == 2)
        //             {
        //                 for (int tmpX = 1; tmpX >= 0; tmpX--)
        //                 {
        //                     tmp_vlt = tmp_adc[tmpX];
        //                     printf("%d\n\r", tmp_vlt);
        //                     // demodulation(tmp_vlt);
        //                 }
        //                 adc_inverse = 0;
        //             }
        //         }
        //         else if (ret == ESP_ERR_TIMEOUT)
        //         {
        //             // We try to read `EXAMPLE_READ_LEN` until API returns timeout, which means there's no available data
        //             break;
        //         }
        //     }
        //     ESP_ERROR_CHECK(adc_continuous_start(handle));
        // }
    }
}
