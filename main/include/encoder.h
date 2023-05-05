#ifndef __ENCODER__
#define __ENCODER__
//GPIO 接线
// A0 -> 32
// A1 -> 33
// B6 -> 22
// B7 -> 23
#define GPIO_ENCODER_OUTPUT_1 32 //(left wheel)
#define GPIO_ENCODER_OUTPUT_2 33
#define GPIO_INPUT_ENCODER_1 22 // (right wheel)
#define GPIO_INPUT_ENCODER_2 23
#define UNIT_TIME 50e3
#define KP 100
#define KI 30

void measure_speed();
void encoder_config();
void encoder_timer_config();
int adjust_wheel_speed();

#endif