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
#include "sys/unistd.h"
#include "esp32/rom/ets_sys.h"
#include "driver/rmt_rx.h"
#include "driver/gpio.h"

static const char *TAG = "example";

#define PD_GPIO_NUM 35

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

// static adc_channel_t channel[1] = {ADC_CHANNEL_7}; // gpio35



void app_main(void)
{
    gpio_config_t io_conf={};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL<<35);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    gpio_config_t led_config={
        .pin_bit_mask = (1<<34),
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_down_en = 0,
        .pull_up_en=0
    };

    gpio_config(&led_config);

    static uint8_t result[3000];
    while(1)
    {
        for(int i = 0; i < 3000; i++)
        {
            result[i] = gpio_get_level(35);
            ets_delay_us(1);
        }
        for(int i = 0; i < 3000; i++)
        {
            printf("%d\n", result[i]);
        }
        printf("-4000\n");

    }
}
