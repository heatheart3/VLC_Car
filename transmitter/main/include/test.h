#ifndef TEST_H
#define TEST_H

#include "light.h"

#define BYTES_PER_FRAME 4

static int test_gpio_num = 0;


/**
 * @brief configure the test program:
 * 1.  configure the GPIO pin
 */
void test_config(const int gpio_num);

/**
* @brief: get a simple square wave
*/
void test_square_wave();
/**
* @brief: get a simple "101100" continue wave, mainly for OOK testing
*/
void test_10_1100();

/**
* @brief: transmit original ascii data, without any encoding.
*/
void test_transmit_original(const char* mes);

void test_transmit_raptor();
#endif