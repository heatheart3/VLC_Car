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
#include "driver/rmt_rx.h"

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


static void example_parse_nec_frame(rmt_symbol_word_t *rmt_nec_symbols, size_t symbol_num)
{
    printf("NEC frame start---\r\n");
    for (size_t i = 0; i < symbol_num; i++) {
        printf("{%d:%d},{%d:%d}\r\n", rmt_nec_symbols[i].level0, rmt_nec_symbols[i].duration0,
               rmt_nec_symbols[i].level1, rmt_nec_symbols[i].duration1);
    }
    printf("---NEC frame end: ");
}

static bool example_rmt_rx_done_callback(rmt_channel_handle_t channel, const rmt_rx_done_event_data_t *edata, void *user_data)
{
    BaseType_t high_task_wakeup = pdFALSE;
    QueueHandle_t receive_queue = (QueueHandle_t)user_data;
    // send the received RMT symbols to the parser task
    xQueueSendFromISR(receive_queue, edata, &high_task_wakeup);
    return high_task_wakeup == pdTRUE;
}

void app_main(void)
{
    ESP_LOGI(TAG, "create RMT RX channel");
    rmt_rx_channel_config_t rx_channel_cfg = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz =1000000,
        .mem_block_symbols = 64, // amount of RMT symbols that the channel can store at a time
        .gpio_num = PD_GPIO_NUM,
    };
    rmt_channel_handle_t rx_channel = NULL;
    ESP_ERROR_CHECK(rmt_new_rx_channel(&rx_channel_cfg, &rx_channel));

 ESP_LOGI(TAG, "register RX done callback");
  QueueHandle_t receive_queue = xQueueCreate(1, sizeof(rmt_rx_done_event_data_t));
    assert(receive_queue);
    rmt_rx_event_callbacks_t cbs = {
        .on_recv_done = example_rmt_rx_done_callback,
    };
    ESP_ERROR_CHECK(rmt_rx_register_event_callbacks(rx_channel,&cbs,receive_queue));
    rmt_receive_config_t receive_config={
        .signal_range_min_ns=10,
        .signal_range_max_ns=2000
    };

    ESP_LOGI(TAG, "enable RMT TX and RX channels");
    ESP_ERROR_CHECK(rmt_enable(rx_channel));
    rmt_symbol_word_t raw_symbols[64];
    rmt_rx_done_event_data_t rx_data;

    ESP_ERROR_CHECK(rmt_receive(rx_channel,raw_symbols,sizeof(raw_symbols),&receive_config));
    while(1)
    {
    // wait for RX done signal
        if (xQueueReceive(receive_queue, &rx_data, pdMS_TO_TICKS(1000)) == pdPASS) {
            // parse the receive symbols and print the result
            example_parse_nec_frame(rx_data.received_symbols, rx_data.num_symbols);
            // start receive again
            ESP_ERROR_CHECK(rmt_receive(rx_channel, raw_symbols, sizeof(raw_symbols), &receive_config));
        }
        else
        {
            printf("waiting for receive!");
        }
    }
}
