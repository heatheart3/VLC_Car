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
#define DOUBLE_HIGH_THRES 12
#define DOUBLE_LOW_THRES 12
#define SINGLE_HIGH_THRES 5
#define SINGLE_LOW_THRES 5

// #define HEADER_THRES 50
// #define DOUBLE_HIGH_THRES 20
// #define DOUBLE_LOW_THRES 20
// #define SINGLE_HIGH_THRES 5
// #define SINGLE_LOW_THRES 5

char decode_data[DECODED_MES_LEN] = {0};
char mes_buffer[VALID_MES_LEN + 4] = {0};
char mes_result[1000] = {0};
int mes_counter = 0;
int decect_header_counter = 0;

int overflow_mark = 0;

enum CAR_STATE current_state = ENUM_STOP;
enum CAR_STATE last_state = ENUM_STOP;

static adc_channel_t channel[1] = {ADC_CHANNEL_7}; // gpio35

extern QueueHandle_t encoder_queue;

void decode(const char *encode, char *data)
{
    int j = 0;
    // printf("Start decode\n");
    // printf("decode_ori:");
    // for (int i = 0; i < VALID_MES_LEN; i++)
    // {
    //     printf("%d", encode[i]);
    // }
    // printf("\n");
    // printf("decode_data:");
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
                // printf("1");
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
                // printf("0");
            }
        }
        j += 2;
    }
    // printf("\n");

    char decode_ascii = 0;
    int tmp_bit = 0;
    for (int i = 0; i < strlen(data); i++)
    {
        tmp_bit = data[i] - '0';
        decode_ascii = decode_ascii | (tmp_bit << (7 - i));
    }
    // printf("%c\n",decode_ascii);
    mes_result[mes_counter] = decode_ascii;
    mes_counter++;
}

void app_main(void)
{

    motor_config();

    servo_config();

    // pcnt_unit_handle_t left_pcnt_unit = NULL;
    // encoder_init(&left_pcnt_unit,LEFT_ENCODER_A,LEFT_ENCODER_B);
    // pcnt_unit_handle_t right_pcnt_unit = NULL;
    // encoder_init(&right_pcnt_unit,RIGHT_ENCODER_A,RIGHT_ENCODER_B);

    turn_forward();

    /*variable for vlc*/
    u_int16_t tmp_vlt;

    uint8_t low_count = 0;
    uint8_t high_count = 0;
    int bit_counter = -1;
    bool mes_start_flg = false;
    /*variable for vlc*/

    esp_err_t ret;
    uint32_t ret_num = 0;
    uint8_t result[EXAMPLE_READ_LEN] = {0};
    memset(result, 0xcc, EXAMPLE_READ_LEN);

    adc_continuous_handle_t handle = NULL;
    adc_continuous_evt_cbs_t adc1_handle;
    adc1_handle.on_pool_ovf = detect_overflow;
    continuous_adc_init(channel, sizeof(channel) / sizeof(adc_channel_t), &handle);
    ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(handle, &adc1_handle, NULL));
    ESP_ERROR_CHECK(adc_continuous_start(handle));

    int pwmA = 60;
    int pwmB = 0;
    int countA = 0;
    int countB = 0;

    bit_counter = -1;
    mes_start_flg = false;
    high_count = 0;
    low_count = 0;
    memset(mes_buffer, 0, sizeof(mes_buffer));
    memset(decode_data, 0, sizeof(decode_data));
    while (1)
    {
        while (1)
        {
            // pcnt_unit_get_count(left_pcnt_unit,&countA);
            // pcnt_unit_get_count(right_pcnt_unit,&countB);
            //             printf("countA:%d\n\r",countA);
            //                         printf("countB:%d\n\r",countB);
            // pwmB = Incremental_PI(countB, countA);
            // pcnt_unit_clear_count(left_pcnt_unit);
            // pcnt_unit_clear_count(right_pcnt_unit);
            // // right_wheel_forward(pwmB);
            // printf("PWMB:%d\n\r",pwmB);

            if (mes_counter >= 1000)
            {
                ESP_ERROR_CHECK(adc_continuous_stop(handle));
                // printf("Start to printf Message\n");
                // printf("Totally dectect %d messages\n", decect_header_counter);
                bit_counter = -1;
                mes_start_flg = false;
                high_count = 0;
                low_count = 0;
                memset(mes_buffer, 0, sizeof(mes_buffer));
                memset(decode_data, 0, sizeof(decode_data));
                for (int i = 0; i < 1000; i++)
                {
                    printf("%c\n", mes_result[i]);
                }
                mes_counter = 0;
                ESP_ERROR_CHECK(adc_continuous_start(handle));
            }
            ret = adc_continuous_read(handle, result, EXAMPLE_READ_LEN, &ret_num, 0);
            if (overflow_mark == 1)
            {
                printf("overflow\n");
                bit_counter = -1;
                mes_start_flg = false;
                high_count = 0;
                low_count = 0;
                memset(mes_buffer, 0, sizeof(mes_buffer));
                memset(decode_data, 0, sizeof(decode_data));
                overflow_mark = 0;
            }
            if (ret == ESP_OK)
            {

                // ESP_LOGI("TASK", "ret is %x, ret_num is %" PRIu32, ret, ret_num);?
                ESP_ERROR_CHECK(adc_continuous_stop(handle));

                int adc_inverse = 0;
                int tmp_adc[2] = {0};
                for (int i = 0; i < ret_num; i += SOC_ADC_DIGI_RESULT_BYTES)
                {
                    adc_digi_output_data_t *p = (void *)&result[i];
                    if (check_valid_data(p))
                    {
                        tmp_adc[adc_inverse] = p->type1.data;
                        adc_inverse++;
                        if (adc_inverse == 2)
                        {
                            for (int tmpX = 1; tmpX >= 0; tmpX--)
                            {
                                tmp_vlt = tmp_adc[tmpX];
                                // printf("%d\n\r", tmp_vlt);
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

                                            // printf("\n");
                                            // printf("oringal!\n");
                                            // Decode Message
                                            decode(mes_buffer, decode_data);
                                            // if (last_state != current_state)
                                            // {
                                            //     last_state = current_state;
                                            //     car_control(decode_data,25);
                                            // }
                                            // car_control(decode_data,24);
                                            mes_start_flg = false;
                                            bit_counter = -1;
                                            memset(mes_buffer, 0, sizeof(mes_buffer));
                                            memset(decode_data, 0, sizeof(decode_data));
                                        }
                                    }
                                }
                            }
                            adc_inverse = 0;
                        }
                    }
                    /**
                     * Because printing is slow, so every time you call `ulTaskNotifyTake`, it will immediately return.
                     * To avoid a task watchdog timeout, add a delay here. When you replace the way you process the data,
                     * usually you don't need this delay (as this task will block for a while).
                     */
                    // vTaskDelay(1);
                    else if (ret == ESP_ERR_TIMEOUT)
                    {
                        // We try to read `EXAMPLE_READ_LEN` until API returns timeout, which means there's no available data
                        break;
                    }
                }
                ESP_ERROR_CHECK(adc_continuous_start(handle));
            }
            //  vTaskDelay(1);
        }

        ESP_ERROR_CHECK(adc_continuous_stop(handle));
        ESP_ERROR_CHECK(adc_continuous_deinit(handle));
    }
}
