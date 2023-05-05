#ifndef __LIGHT__H__
#define __LIGHT__H__

#include "./driver/ledc.h"
#include "./driver/gpio.h"
#include <string.h>

#define GPIO_OUTPUT_LIGHT 15


void light_ook_config();
void light_pwm_config();


/**
 * @brief Transmit OOK data
 * @param data: char pointer to data to be transmitted
 */
void transmit_ook(const char *data);

#endif