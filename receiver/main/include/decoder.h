#ifndef __DECODER__
#define __DECODER__
#include <stdio.h>
#include "./include/myadc.h"
#include "esp_log.h"
#include <string.h>


void decoder(uint8_t *data, int data_num, uint8_t* decode_data,  uint16_t* decode_data_num);
uint8_t decode_half_per(uint16_t avg);
void clean_data(uint8_t *data, int data_num, uint16_t *result_data);
void decode(uint16_t *raw_data,int data_num, uint8_t* decode_data,  uint16_t* decode_data_num);
bool collect_data(uint8_t *data, int data_num, uint16_t* collected_raw_data, uint16_t collected_data_array_size);




#endif