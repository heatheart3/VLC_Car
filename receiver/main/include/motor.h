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
#include <string.h>

#define GPIO_OUTPUT_AIN1    14 //AIN1 PB5
#define GPIO_OUTPUT_BIN1    12 //BIN1 PB4
#define GPIO_OUTPUT_AIN2    19 //AIN2 PA15
#define GPIO_OUTPUT_BIN2    17 //BIN2 PA12
#define GPIO_OUTPUT_PWMA   18 //PWMA PA11
#define GPIO_OUTPUT_PWMB   16 //PWMB PA8
#define DUTY_RESOLUTION LEDC_TIMER_13_BIT

#define STOP "110"
#define FORWARD "101"
#define BACKWARD "010"
#define LEFT   "001"
#define RIGHT  "100"

void motor_config();
int perDuty2Duty(int perDuty);
void forward(int per);
void backward(int per);
void left_forward(int per);
void right_forward(int per);
void stop();

void car_control(const char* cmd);


#endif