#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/pulse_cnt.h"
#include "driver/gpio.h"

#define PCNT_HIGH_LIMIT 10000
#define PCNT_LOW_LIMIT -10000

#define LEFT_ENCODER_A 5
#define LEFT_ENCODER_B 15

#define RIGHT_ENCODER_A 22
#define RIGHT_ENCODER_B 23

#define TARGET 20
#define Velocity_KP 0.2
#define Velocity_KI 0.3

static float last_bias;

void encoder_init(pcnt_unit_handle_t* pcnt_unit,int a,int b);

int Incremental_PI(int Encoder, int Target);