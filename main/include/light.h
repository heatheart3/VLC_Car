#ifndef __LIGHT__
#define __LIGHT__
#include "./driver/ledc.h"
#include "./driver/gpio.h"

#define GPIO_OUTPUT_LIGHT 15

void light_ook_config();
void light_pwm_config();


#endif