#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include "./include/vlc_esp32_hardware.h"

#define ASCII_MES_LEN MANCHESTER_SYMBOLS_LEN/8

void test_print_PHY_symbols_buffer(uint8_t *buffer, uint32_t length);

void test_get_ASCII(const uint8_t* symbols, uint8_t* ch);

#endif