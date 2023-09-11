#ifndef __SPINAL_CODE_CONFIG_H__
#define __SPINAL_CODE_CONFIG_H__


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

#endif