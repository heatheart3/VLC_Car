#ifndef VLC_ESP32_HARDWARE_H
#define VLC_ESP32_HARDWARE_H

#include <stdio.h>
#include "esp32/rom/ets_sys.h"
#include "driver/gpio.h"


void PHY_gpio_config(const int gpio_num);

void PHY_read_symbols(uint8_t *buffer, uint16_t length,const int PD_GPIO_NUM);

#endif