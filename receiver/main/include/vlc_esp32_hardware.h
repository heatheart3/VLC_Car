#ifndef VLC_ESP32_HARDWARE_H
#define VLC_ESP32_HARDWARE_H

#include "./include/decoder.h"
#include "./include/mytimer.h"
#include <stdio.h>
#include "esp32/rom/ets_sys.h"
#include "driver/gpio.h"

extern  uint8_t overflow_symbol_counter;

extern uint64_t s_count;
extern uint64_t e_count;
extern gptimer_handle_t gptimer;



/**
 * @brief Configures a GPIO pin as input
 */
void PHY_gpio_config(const int gpio_num);


/**
 * @brief Reads a buffer of symbols from a GPIO pin
 * @param buffer: pointer to the buffer where the symbols will be stored
 * @param length: number of symbols to be read
 */
void PHY_read_symbols(uint8_t *buffer, uint16_t length,const int PD_GPIO_NUM);

uint8_t PHY_demoluate_OOK(const uint8_t *buffer, uint16_t *start_index, const uint16_t length, uint8_t *mes_buffer);

void PHY_decode_manchester(const uint8_t *symbols, uint8_t *mes);

void PHY_decode_networkcoding(const uint8_t *symbols, uint8_t *mes);


void PHY_decode_spinal(const uint8_t *symbols, uint8_t *mes);


void  PHY_decode_allinone(const uint8_t *symbols, uint8_t *mes,uint8_t* symbolsB);

uint8_t PHY_demoluate_OOK_spinal(const uint8_t *buffer, uint16_t *start_index, const uint16_t length, uint8_t *mes_buffer,uint8_t *header_length);
uint8_t PHY_demoluate_OOK_SpinalV2(const uint8_t *buffer, uint16_t *start_index, const uint16_t length, uint8_t *mes_buffer);
uint8_t PHY_demoluate_OOK_Ver2(const uint8_t *buffer, uint16_t *start_index, const uint16_t length, uint8_t *mes_buffer);
#endif