//
// Created by Administrator on 2023/6/21.
//

#ifndef TEST_CONSTANT_H
#define TEST_CONSTANT_H



/**
 * @brief  It's similar to sampling rate.
 */
#define SAMPLING_DURATION 4





/**
 * @brief Spinal Code parameters
 */
#define K 3 //<=3, or the heap will overlap.
#define C 4
#define B 4
#define D 2 
#define PASS 1

/**
 * @brief  Spianl Code Decoder intermediate parameters
 */
#define MES_LENGTH 4  //MAX_LENGTH is 8, because UINT64_MAX is 8 bytes
#define WAVEFRONT_MAX B*(1<<K)*(1<<K)
#define SUBTREES_MAX B*(1<<K)
#define SPINE_LENGTH ((8*MES_LENGTH+K-1)/K)
#define PASS_LENGTH (SPINE_LENGTH*PASS)


/**
 * @brief Demodulation parameters
 */
#define HEADER_THRES 12
#define DOUBLE_HIGH_THRES 7
#define DOUBLE_LOW_THRES 7
#define SINGLE_HIGH_THRES 2
#define SINGLE_LOW_THRES 2

// #define OOK_SYMBOLS_LEN (PASS_LENGTH*C*2)
#define OOK_SYMBOLS_LEN 64
#define MANCHESTER_SYMBOLS_LEN (OOK_SYMBOLS_LEN/2)


#define ASCII_MES_LEN 4


#endif //TEST_CONSTANT_H
