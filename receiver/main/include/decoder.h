#ifndef __DECODER__
#define __DECODER__
#include <stdio.h>
#include "./include/myadc.h"



void decode(uint8_t *data, int data_num, uint8_t* decode_data,  uint16_t* decode_data_num);
uint8_t decode_half_per(uint16_t avg);

#endif