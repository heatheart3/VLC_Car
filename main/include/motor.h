#ifndef __MOTOR__
#define __MOTOR__

/* A (right wheel):
    AIN1   AIN2     Direction
    0       1       forward
    1       0       backward
    0       0       stop
*/
/*
    B (left wheel):
    BIN1   BIN2     Direction
    1       0       forward
    0       1       backward
    0       0       stop
*/

#include "./driver/gpio.h"
#include "./driver/ledc.h"

#define GPIO_OUTPUT_AIN1    14 //AIN1
#define GPIO_OUTPUT_BIN1    12 //BIN1
#define GPIO_OUTPUT_AIN2    19 //AIN2
#define GPIO_OUTPUT_BIN2    17 //BIN2
#define GPIO_OUTPUT_PWMA   18 //PWMA(right wheel)
#define GPIO_OUTPUT_PWMB   16 //PWMB(left wheel)
#define DUTY_RESOLUTION LEDC_TIMER_13_BIT

void motor_config();
int perDuty2Duty(int perDuty);
void forward(int per);
void backward(int per);

void forward_PID(int speed_new);
void stop();


#endif