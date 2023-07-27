#ifndef TEST_H
#define TEST_H


#include "./include/constant.h"
#include "./include/encoder.h"
#include "./include/light.h"


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include<stdio.h>
#include <string.h>
#include <sys/time.h>

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