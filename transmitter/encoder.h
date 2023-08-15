#ifndef ENCODER_H
#define ENCODER_H

#include "./include/RNG.h"
#include "./include/Lookup3Hash.h"
#include "./include/constant.h"

#include<stdint.h>
#include<string.h>
#include<stdlib.h>

/*
 * SpianlCode encode.
 * message : oringinal message;
 * @return: return transmitting symbols;
*/
uint8_t* SpinalEncode(const char* message);

/**
 * Network coding.
 * symbolsA: symbols from A;
 * symbolsB: symbols from B;
 * @return: return XOR mixed symbols;
 **/
uint8_t* network_coding(uint8_t* symbolsA, uint8_t* symbolsB);
#endif
