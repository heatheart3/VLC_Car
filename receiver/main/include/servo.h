#ifndef __SERVO__
#define __SERVO__
#include "./driver/ledc.h"

#define GPIO_OUTPUT_PWM_SERVO 13
#define SERVO_FREQ 50

//using 1024*ms as duty. 1ms is 0 degree, 1.5ms is 90 degree, 2ms is 180 degree
#define SERVO_MIN_DUTY 0
#define SERVO_MAX_DUTY 102
#define SERVO_MID_DUTY 76

void servo_config();

void turn_left45();

void turn_right45();

void turn_forward();

void turn_angle(const uint8_t angle);

#endif