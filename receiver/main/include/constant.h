//
// Created by Administrator on 2023/6/21.
//

#ifndef TEST_CONSTANT_H
#define TEST_CONSTANT_H

#define MES_LENGTH 4  //MAX_LENGTH is 8, because UINT64_MAX is 8 bytes

//Spinalcodes parameters
#define K 3 //<=3, or the heap will overlap.
#define C 6 
#define B 4
#define D 2 
#define PASS 3

#define WAVEFRONT_MAX B*(1<<K)*(1<<K)
#define SUBTREES_MAX B*(1<<K)
#define SPINE_LENGTH ((8*MES_LENGTH+K-1)/K)
#define PASS_LENGTH (SPINE_LENGTH*PASS)

#endif //TEST_CONSTANT_H