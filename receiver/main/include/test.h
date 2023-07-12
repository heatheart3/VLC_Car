#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include "./include/vlc_esp32_hardware.h"
#include "./include/constant.h"


static uint32_t packet_contuer=0;
static uint8_t mes_buffer[OOK_SYMBOLS_LEN];
static uint8_t manchester_symbols[MANCHESTER_SYMBOLS_LEN];
static uint8_t ascii_mes[ASCII_MES_LEN];

void test_print_PHY_symbols_buffer(uint8_t *buffer, uint32_t length);

void test_get_ASCII(const uint8_t* symbols, uint8_t* ch);

void test0_get_packet(uint8_t* symbols_buffer,uint32_t symbols_length);

void test1_get_packet_spinal(uint8_t* symbols_buffer,uint32_t symbols_length);
#endif