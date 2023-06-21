#ifndef __DECODER__
#define __DECODER__
#include <stdio.h>
#include "./include/myadc.h"
#include "esp_log.h"
#include <string.h>


void decoder(uint8_t *data, int data_num, uint8_t* decode_data,  uint16_t* decode_data_num);
uint8_t decode_half_per(uint16_t avg);
void clean_data(uint8_t *data, int data_num, uint16_t *result_data);
uint16_t avg_caculate(uint16_t left, uint16_t right, uint16_t *raw_data, uint16_t data_num);
void raw_data_avg_caculate(uint16_t *raw_data, int data_num, uint16_t *result_data, uint16_t *result_data_num);
void decode(uint16_t *raw_data,int data_num, uint8_t* decode_data,  uint16_t* decode_data_num);
bool collect_data(uint8_t *data, int data_num, uint16_t* collected_raw_data, uint16_t collected_data_array_size);




#endif