/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "./include/mytimer.h"
#include "./include/myadc.h"
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
#define VALID_MES_LEN (PASS_LENGTH * 12)

#define HEADER_THRES 20
#define DOUBLE_HIGH_THRES 14
#define DOUBLE_LOW_THRES 14
#define SINGLE_HIGH_THRES 5
#define SINGLE_LOW_THRES 5

int overflow_mark = 0;

/*variable for vlc demodulation*/
static u_int16_t tmp_vlt = 0;
static uint8_t low_count = 0;
static uint8_t high_count = 0;
static int bit_counter = -1;
static char mes_start_flg = 0;
/*variable for vlc demodulation*/

static uint8_t mes_buffer[VALID_MES_LEN] = {0};
static uint8_t symbolsA[PASS_LENGTH] = {0};
static uint8_t symbolsB[] = {34, 12, 13, 59, 17, 18, 59, 26, 45, 13, 46, 48, 24, 63, 11, 34, 54, 42, 55, 35, 0, 48, 4, 36, 47, 21, 59, 54, 24, 58, 4, 58, 44};
static uint8_t decode_data[4] = {0};

static adc_channel_t channel[1] = {ADC_CHANNEL_7}; // gpio35


static uint64_t mes_start_time=0;
static uint64_t demodulation_time =0;
static uint64_t mes_end_time=0;
static uint64_t deOOK_start_time=0;
static uint64_t deOOK_end_time=0;
static uint64_t despinal_start_time=0;
static uint64_t despinal_end_time=0;
static uint64_t denetwork_start_time=0;
static uint64_t denetwork_end_time=0;
static gptimer_handle_t gptimer =NULL;
void demodulation(const int tmp_vlt)
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
                // printf("mes start!\n");
                ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &mes_start_time));
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
                ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &demodulation_time));
                // Decode Message
                ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &deOOK_start_time));
                decode_OOK(mes_buffer, symbolsA);
                ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &deOOK_end_time));

                ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &denetwork_start_time));
                network_decode(symbolsA, symbolsB);
                ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &denetwork_end_time));

                ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &despinal_start_time));
                SpinalDecode(symbolsA, decode_data);
                ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &despinal_end_time));

                ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &mes_end_time));
                for (int i = 0; i < 4; i++)
                {
                    printf("%c", decode_data[i]);
                }
                printf("\n");
                printf("demodulation time: %llu\n", demodulation_time - mes_start_time);
                printf("deOOK time: %llu\n", deOOK_end_time - deOOK_start_time);
                printf("denetwork time: %llu\n", denetwork_end_time - denetwork_start_time);
                printf("despinal time: %llu\n", despinal_end_time - despinal_start_time);
                printf("mes time: %llu\n", mes_end_time - mes_start_time);
                mes_start_flg = false;
                bit_counter = -1;
                memset(mes_buffer, 0, sizeof(mes_buffer));
            }
        }
    }
}

void print_VLT(esp_err_t ret, uint32_t ret_num, uint8_t *result, adc_continuous_handle_t handle)
{
    // if the adc read is successful
    if (ret == ESP_OK)
    {
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
                        printf("%d\n\r", tmp_vlt);
                    }
                    adc_inverse = 0;
                }
            }
        }
        ESP_ERROR_CHECK(adc_continuous_start(handle));
    }
    else if (ret == ESP_ERR_TIMEOUT)
    {
        printf("ADC capture timeout\n");
    }
}

void recover_message(esp_err_t ret, uint32_t ret_num, uint8_t *result, adc_continuous_handle_t handle)
{
    // if the adc read is successful
    if (ret == ESP_OK)
    {
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
                        demodulation(tmp_vlt);
                    }
                    adc_inverse = 0;
                }
            }
        }
    }
    // else if (ret == ESP_ERR_TIMEOUT)
    // {
    //     printf("ADC capture timeout\n");
    // }
}

void app_main(void)
{

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

    gptimer_config_t gptimer_config={
        .clk_src=GPTIMER_CLK_SRC_DEFAULT,
        .direction=GPTIMER_COUNT_UP,
        .resolution_hz=1*1000*1000,
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&gptimer_config,&gptimer));
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));
    while (1)
    {

        ret = adc_continuous_read(handle, result, EXAMPLE_READ_LEN, &ret_num, 0);

        // detect the overflow
        if (overflow_mark == 1)
        {
            printf("overflow\n");
            bit_counter = -1;
            mes_start_flg = false;
            high_count = 0;
            low_count = 0;
            memset(mes_buffer, 0, sizeof(mes_buffer));
            overflow_mark = 0;
        }

        recover_message(ret, ret_num, result, handle);
    }
}
