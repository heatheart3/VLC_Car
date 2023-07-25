#ifndef TEST_H
#define TEST_H


#include "./include/mytimer.h"
#include <stdio.h>
#include "./include/vlc_esp32_hardware.h"
#include "./include/constant.h"
#include <string.h>

extern uint8_t decode_right_conuter;
extern uint8_t overflow_symbol_counter;

extern uint64_t s_count;
extern uint64_t e_count;
extern uint64_t s_error_count;
extern gptimer_handle_t gptimer;

void test_print_PHY_symbols_buffer(uint8_t *buffer, uint32_t length);

void test_get_ASCII(const uint8_t* symbols, uint8_t* ch);

void test0_get_packet(uint8_t* symbols_buffer,uint32_t symbols_length);
uint32_t test0_get_packet_ver2(uint8_t *symbols_buffer, uint32_t symbols_length);

void test1_get_packet_spinal(uint8_t* symbols_buffer,uint32_t symbols_length);
uint32_t test1_get_packet_spinal_ver2(uint8_t *symbols_buffer, uint32_t symbols_length);
void  test2_get_overall_latency(uint8_t *symbols_buffer, uint32_t symbols_length,uint8_t* symbolsB);


void test_get_raptor(uint8_t* symbols_buffer,uint32_t symbols_length);
#endif