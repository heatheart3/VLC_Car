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

void test0_transmit_nonencode(const char* mes);
void test0_transmit_nonencode_ver2(const char *mes);

void test1_transmit_spinal(const char* mes);

void test_square_wave();

#endif