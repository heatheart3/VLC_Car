#ifndef TEST_H
#define TEST_H


#include "./include/mytimer.h"
#include <stdio.h>
#include "./include/vlc_esp32_hardware.h"
#include "./include/constant.h"
#include <string.h>

extern uint8_t decode_right_conuter;
extern uint8_t overflow_symbol_counter;
extern int pass_counter;

extern uint64_t s_count;
extern uint64_t e_count;
extern uint64_t s_error_count;
extern gptimer_handle_t gptimer;

static uint8_t mes_buffer[OOK_SYMBOLS_LEN];
static uint8_t spinal_mes_buffer[OOK_SYMBOLS_LEN*PASS];
static uint8_t manchester_symbols[MANCHESTER_SYMBOLS_LEN];
static uint8_t ascii_mes[MES_LENGTH];

static uint8_t output_buffer[1]={0};


void test_print_PHY_symbols_buffer(uint8_t *buffer, uint32_t length);

void test_get_ASCII(const uint8_t* symbols, uint8_t* ch);

void test0_get_packet(uint8_t* symbols_buffer,uint32_t symbols_length);
void test0_get_packet_ver2(uint8_t *symbols_buffer, uint32_t symbols_length);

void test1_get_packet_spinal(uint8_t* symbols_buffer,uint32_t symbols_length);
void test1_get_packet_spinal_ver2(uint8_t *symbols_buffer, uint32_t symbols_length);
void  test2_get_overall_latency(uint8_t *symbols_buffer, uint32_t symbols_length,uint8_t* symbolsB);

uint32_t test2_get_packet_non_throughput(uint8_t *symbols_buffer, uint32_t symbols_length);
uint32_t test2_get_packet_sc_throughput(uint8_t *symbols_buffer, uint32_t symbols_length);
#endif