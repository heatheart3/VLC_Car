//
// Created by Administrator on 2023/6/21.
//

#ifndef TEST_CONSTANT_H
#define TEST_CONSTANT_H



#define SHOW_LATENCY 1


/**
 * @brief  It's similar to sampling rate.
 */
#define SAMPLING_DURATION 2
#define SIGNAL_DURATION 12


#define FRAME_LENGTH 5   // the length of a frame (5 char)=1 PSN + 4 Datass
#define CHAR_PER_FRAME 4 //the number of char in a frame


#define MES_LENGTH 50 //the whole message length

/**
 * @brief Spinal Code parameters
 */
#define K 3 //<=3, or the heap will overlap.
#define C 5
#define B 4
#define D 2 
#define PASS 1

/**
 * @brief  Spianl Code Decoder intermediate parameters
 */

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

/******** 20KHz Sampling Rate 2 ********/
// #define HEADER_THRES 80
// #define HEADER_LOW_THRES 100
// #define DOUBLE_HIGH_THRES 38
// #define DOUBLE_LOW_THRES 38
// #define SINGLE_HIGH_THRES 2
// #define SINGLE_LOW_THRES 2
// #define SYMBOLS_BUFFER_SIZE2 2000

/******** 40KHz Sampling Rate 2 ********/
// #define HEADER_THRES 37
// #define HEADER_LOW_THRES 54
// #define DOUBLE_HIGH_THRES 18
// #define DOUBLE_LOW_THRES 18
// #define SINGLE_HIGH_THRES 2
// #define SINGLE_LOW_THRES 2
// #define SYMBOLS_BUFFER_SIZE2 1500


/******** 60KHz Sampling Rate 2 ********/
// #define HEADER_THRES 24
// #define HEADER_LOW_THRES 35
// #define DOUBLE_HIGH_THRES 11
// #define DOUBLE_LOW_THRES 11
// #define SINGLE_HIGH_THRES 2
// #define SINGLE_LOW_THRES 2
// #define SYMBOLS_BUFFER_SIZE2 1000
/******** 80KHz Sampling Rate 2 ********/
#define HEADER_THRES 18
#define HEADER_LOW_THRES 25
#define DOUBLE_HIGH_THRES 8
#define DOUBLE_LOW_THRES 8
#define SINGLE_HIGH_THRES 2
#define SINGLE_LOW_THRES 2
#define SYMBOLS_BUFFER_SIZE2 460
/******** 100KHz Sampling Rate 2 ********/
// #define HEADER_THRES 15
// #define HEADER_LOW_THRES 20
// #define DOUBLE_HIGH_THRES 7
// #define DOUBLE_LOW_THRES 7
// #define SINGLE_HIGH_THRES 2
// #define SINGLE_LOW_THRES 2
// #define SYMBOLS_BUFFER_SIZE2 420


// #define OOK_SYMBOLS_LEN (SPINE_LENGTH*C*2)
#define OOK_SYMBOLS_LEN 80 // SSN (1 char) + Payload (4 char)
#define MANCHESTER_SYMBOLS_LEN (OOK_SYMBOLS_LEN/2)


#endif //TEST_CONSTANT_H
