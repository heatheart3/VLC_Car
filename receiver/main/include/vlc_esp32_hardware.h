#ifndef VLC_ESP32_HARDWARE_H
#define VLC_ESP32_HARDWARE_H

#include <stdio.h>
#include "esp32/rom/ets_sys.h"
#include "driver/gpio.h"

#define SAMPLING_DURATION 5

#define HEADER_THRES 12
#define DOUBLE_HIGH_THRES 5
#define DOUBLE_LOW_THRES 5
#define SINGLE_HIGH_THRES 2
#define SINGLE_LOW_THRES 2

#define VALID_MES_LEN 4


void PHY_gpio_config(const int gpio_num);

void PHY_read_symbols(uint8_t *buffer, uint16_t length,const int PD_GPIO_NUM);

void PHY_demoluate_OOK(const uint8_t *buffer, uint16_t *start_index, const uint16_t length, uint8_t *mes_buffer);

#endif