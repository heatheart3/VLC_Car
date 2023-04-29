#ifndef __SERVO__
#define __SERVO__
#include "./driver/ledc.h"

#define GPIO_OUTPUT_PWM_SERVO 13
#define SERVO_FREQ 50

void servo_config();
void turn_left45();
void turn_right45();
void turn_forward();

#endif