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

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define VLT_HIGH 2700
#define VLT_LOW 0
#define DECODED_MES_LEN 3
#define VALID_MES_LEN 6
char decode_data[DECODED_MES_LEN] = {0};
char mes_buffer[20] = {0};

static adc_channel_t channel[1] = {ADC_CHANNEL_6}; // gpio34

void decode(const char *encode, char *data)
{
    int j = 0;
    printf("decode_ori:");
    for (int i = 0; i < VALID_MES_LEN; i++)
    {
        printf("%d", encode[i]);
    }
    printf("\n");
    printf("decode_data:");
    for (int i = 0; i < DECODED_MES_LEN; i++)
    {
        if (encode[j] == 0)
        {
            if (encode[j + 1] == 0)
            {
                printf("Message Error!");
                break;
            }
            else
            {
                data[i] = '1';
                printf("1");
            }
        }
        else
        {
            if (encode[j + 1] == 1)
            {
                printf("Message Error!");
                break;
            }
            else
            {
                data[i] = '0';
                printf("0");
            }
        }
        j += 2;
    }
    printf("\n");
}

void app_main(void)
{

    motor_config();

    servo_config();

    turn_forward();

    /*variable for vlc*/
    u_int16_t tmp_vlt;

    uint8_t low_count = 0;
    uint8_t high_count = 0;

    bool mes_start_flg = false;
    /*variable for vlc*/

    esp_err_t ret;
    uint32_t ret_num = 0;
    uint8_t result[EXAMPLE_READ_LEN] = {0};
    memset(result, 0xcc, EXAMPLE_READ_LEN);

    adc_continuous_handle_t handle = NULL;
    continuous_adc_init(channel, sizeof(channel) / sizeof(adc_channel_t), &handle);
    ESP_ERROR_CHECK(adc_continuous_start(handle));

    while (1)
    {

        while (1)
        {
            ret = adc_continuous_read(handle, result, EXAMPLE_READ_LEN, &ret_num, 0);
            if (ret == ESP_OK)
            {
                // ESP_LOGI("TASK", "ret is %x, ret_num is %" PRIu32, ret, ret_num);
                int j = -1;

                mes_start_flg = false;
                high_count = 0;
                low_count = 0;
                memset(mes_buffer, 0, sizeof(mes_buffer));
                memset(decode_data, 0, sizeof(decode_data));
                for (int i = 0; i < ret_num; i += SOC_ADC_DIGI_RESULT_BYTES)
                {
                    adc_digi_output_data_t *p = (void *)&result[i];
                    if (check_valid_data(p))
                    {
                        tmp_vlt = p->type1.data;
                        // printf("%d\r\n", tmp_vlt);
                        if (!mes_start_flg)
                        {
                            if (tmp_vlt >= 2200)
                            {
                                high_count++;
                            }
                            if (tmp_vlt <= VLT_LOW)
                            {
                                if (high_count >= 60)
                                {
                                    mes_start_flg = true;
                                    high_count = 0;
                                    low_count = 0;
                                    j = -1;
                                }
                                else
                                {
                                    high_count = 0;
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
                                if (j == -1)
                                {

                                    j++;

                                    if (low_count >= 28)
                                    {
                                        mes_buffer[j] = 0;
                                        j++;
                                    }
                                    low_count = 0;
                                }
                                else if (j < VALID_MES_LEN)
                                {
                                    if (high_count > low_count)
                                    {
                                        if (high_count >= 30)
                                        {
                                            mes_buffer[j] = 1;
                                            j++;
                                            mes_buffer[j] = 1;
                                            j++;
                                            // printf("11");
                                            high_count = 0;
                                        }
                                        else if (high_count >= 10 && high_count < 30)
                                        {
                                            mes_buffer[j] = 1;
                                            j++;
                                            // printf("1");
                                            high_count = 0;
                                        }
                                        else
                                        {
                                            high_count = 0;
                                            low_count = 0;
                                        }
                                    }
                                    else
                                    {
                                        if (low_count >= 28)
                                        {
                                            mes_buffer[j] = 0;
                                            j++;
                                            mes_buffer[j] = 0;
                                            j++;
                                            // printf("00");
                                            low_count = 0;
                                        }
                                        else if (low_count >= 10 && low_count < 28)
                                        {
                                            mes_buffer[j] = 0;
                                            j++;
                                            // printf("0");
                                            low_count = 0;
                                        }
                                        else
                                        {
                                            low_count = 0;
                                            high_count = 0;
                                        }
                                    }
                                }

                                // OOK Demodulation Done
                                // Codes about control car are written below
                                else
                                {
                                    mes_start_flg = false;
                                    // printf("\n");
                                    // printf("oringal!\n");
                                    high_count = 0;
                                    low_count = 0;
                                    // Decode Message
                                    decode(mes_buffer, decode_data);
                                    car_control(decode_data);
                                    memset(mes_buffer, 0, sizeof(mes_buffer));
                                    memset(decode_data, 0, sizeof(decode_data));
                                }
                            }
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
            }
            //  vTaskDelay(1);
        }

        ESP_ERROR_CHECK(adc_continuous_stop(handle));
        ESP_ERROR_CHECK(adc_continuous_deinit(handle));
    }
}
