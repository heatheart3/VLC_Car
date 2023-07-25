//
// Created by Administrator on 2023/6/21.
//

#ifndef TEST_CONSTANT_H
#define TEST_CONSTANT_H



/**
 * @brief  It's similar to sampling rate.
 */
#define SAMPLING_DURATION 2





/**
 * @brief Spinal Code parameters
 */
#define K 3 //<=3, or the heap will overlap.
#define C 5
#define B 4
#define D 2 
#define PASS 5

/**
 * @brief  Spianl Code Decoder intermediate parameters
 */
// #define MES_LENGTH 8 //MAX_LENGTH is 8, because UINT64_MAX is 8 bytes
//for raptor code
#define MES_LENGTH 5

#if (D==1)
#define WAVEFRONT_MAX B*(1<<(K*(D)))*(1<<(K*(D)))
#define SUBTREES_MAX B*(1<<(K*(D)))
#else
#define WAVEFRONT_MAX B*(1<<(K*(D-1)))*(1<<(K*(D-1)))
#define SUBTREES_MAX B*(1<<(K*(D-1)))
#endif
#define SPINE_LENGTH ((8*MES_LENGTH+K-1)/K)
#define PASS_LENGTH (SPINE_LENGTH*PASS)


/**
 * @brief Demodulation parameters
 */
// #define HEADER_THRES 12
// #define HEADER_LOW_THRES 20
// #define DOUBLE_HIGH_THRES 7
// #define DOUBLE_LOW_THRES 7
// #define SINGLE_HIGH_THRES 2
// #define SINGLE_LOW_THRES 2


/******** 40KHz Sampling Rate 2 ********/
#define HEADER_THRES 37
#define HEADER_LOW_THRES 54
#define DOUBLE_HIGH_THRES 18
#define DOUBLE_LOW_THRES 18
#define SINGLE_HIGH_THRES 2
#define SINGLE_LOW_THRES 2

// #define OOK_SYMBOLS_LEN (SPINE_LENGTH*C*2)
// #define OOK_SYMBOLS_LEN 128

//for raptor codes
// SSN (1 char) + Payload (4 char)
#define OOK_SYMBOLS_LEN 80
#define MANCHESTER_SYMBOLS_LEN (OOK_SYMBOLS_LEN/2)


#endif //TEST_CONSTANT_H
