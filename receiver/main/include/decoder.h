#ifndef DECODER_H
#define DECODER_H


#include "./include/RNG.h"
#include "./include/Lookup3Hash.h"
#include "./include/constant.h"
#include<stdint.h>

#define min(a,b) (((a)<(b))?(a):(b))


typedef struct
{
    int path_metric;
    uint32_t spine_value;
    uint8_t  path[SPINE_LENGTH];
}Wavefront;


/*
 * SpinalCode decode
 */
void SpinalDecode(const uint8_t * symbols, uint8_t * decoded_message);

void decode_OOK(const uint8_t *OOK, uint8_t *symbols);

void network_decode(uint8_t * NC, uint8_t* symbols);


#endif