#ifndef TEST_H
#define TEST_H


#include "./include/mytimer.h"
#include <stdio.h>
#include "./include/vlc_esp32_hardware.h"
#include "./include/constant.h"
#include <string.h>


extern bool timer_isr_flag;
extern uint64_t s_count;
extern uint64_t e_count;
extern uint64_t s_error_count;
extern gptimer_handle_t gptimer;

extern uint64_t used_frame;

/**
 * @brief print all the OOK signals to the screen
 */
void test_print_PHY_symbols_buffer(uint8_t *buffer, uint32_t length);


/**
 * @brief get the original frame from PHY symbols buffer
 */
void test_get_frame(uint8_t* symbols_buffer,uint32_t symbols_length);


void test_get_original_mes(uint8_t* symbols_buffer,uint32_t symbols_length);

#endif