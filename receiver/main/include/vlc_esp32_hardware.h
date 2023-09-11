#ifndef VLC_ESP32_HARDWARE_H
#define VLC_ESP32_HARDWARE_H

#include "./include/decoder.h"
#include "./include/mytimer.h"
#include <stdio.h>
#include "esp32/rom/ets_sys.h"
#include "driver/gpio.h"

extern bool timer_isr_flag;
extern uint64_t s_count;
extern uint64_t e_count;
extern gptimer_handle_t gptimer;



/**
 * @brief Configures a GPIO pin as input
 */
void PHY_gpio_config(const int gpio_num);


/**
 * @brief Reads a buffer of symbols from a GPIO pin, only symbols after frame header will be read
 * @param buffer: pointer to the buffer where the symbols will be stored
 * @param length: number of symbols to be read
 */
void PHY_read_symbols(uint8_t *buffer, uint16_t length,const int PD_GPIO_NUM);

/**
 * @brief Reads a buffer of symbols from a GPIO pin, but all symbols will be read
 */
void PHY_easy_read_symbols(uint8_t *buffer, uint16_t length, const int PD_GPIO_NUM);

/**
 * @brief Decode data from manchester encoding.
 */
void PHY_decode_manchester(const uint8_t *symbols, uint8_t *mes);


/**
 * @brief Decode data from network encoding.
 */
void PHY_decode_networkcoding(const uint8_t *symbols, uint8_t *mes);


/**
 * @brief Decode data from spinal encoding.
 * @param symbols: pointer to the spinal codes symbols, each array element is a symbol
 * @param mes: pointer to the buffer where the decoded data will be stored
 */
void PHY_decode_spinal(const uint8_t *symbols, uint8_t *mes);


/**
 * @brief Demodulate bits from OOK. Using up edge, down edge and counter threshold to determine the bit value.
 * @param buffer: pointer to the buffer where the OOK singals buffer, which start at the last 4 bits of the header: "0000"(11110000)
 * @param length: number of symbols to be demodulated
 * @param mes_buffer: pointer to the buffer where the demodulated data will be stored
 * @warning the bits shift error will happen because of the OOK demodulation method.
 */
uint8_t PHY_demodulate_OOK(const uint8_t *buffer, const uint16_t length, uint8_t *mes_buffer);
#endif