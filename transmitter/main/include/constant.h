//
// Created by Administrator on 2023/6/21.
// This file is used to define the constant parameters of the spinalcodes.
//

#ifndef TEST_CONSTANT_H
#define TEST_CONSTANT_H



/* Parameters for LED Phisycal Layer*/
#define GPIO_LEFT_LIGHT 26
#define GPIO_RIGHT_LIGHT 27

#define  FRAME_HEADER "11110000"

#define CHAR_PER_FRAME 4 // each physical frame has 4 chars, 32 bits


// #define TRANSMIT_PERIOD 50  //20KHz
// #define TRANSMIT_PERIOD 25  //40KHz
// #define TRANSMIT_PERIOD 16  //60KHz
#define TRANSMIT_PERIOD 12  //80KHz
// #define TRANSMIT_PERIOD 10 // 100KHz not useful

/* Parameters for LED Phisycal Layer*/

#define MES_LENGTH 8  //MAX_LENGTH is 8, because UINT64_MAX is 8 bytes

//Spinalcodes parameters
#define K 3 //<=3, or the heap will overlap.
#define C 5
#define B 4
#define D 2 
#define PASS 1

#define WAVEFRONT_MAX B*(1<<K)*(1<<K)
#define SUBTREES_MAX B*(1<<K)
#define SPINE_LENGTH ((8*MES_LENGTH+K-1)/K)
#define PASS_LENGTH (PASS*SPINE_LENGTH)

#endif //TEST_CONSTANT_H
