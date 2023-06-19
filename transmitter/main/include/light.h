#ifndef __LIGHT__H__
#define __LIGHT__H__

#include "./driver/ledc.h"
#include "./driver/gpio.h"
#include <string.h>

#define GPIO_LEFT_LIGHT 26
#define GPIO_RIGHT_LIGHT 27

#define  MES_HEADER "011110"



//CLOCK=16
//10=195KHz
//50= 50KHz
//100=25KHz
#define TRANSMIT_PERIOD 50 //US


enum CAR_STATE{
    ENUM_STOP,
    ENUM_FORWARD,
    ENUM_BACKWARD,
    ENUM_LEFT,
    ENUM_RIGHT
};




/**
 * @brief Configure GPIO for OOK transmission
 * @param gpio_num: GPIO number of the pin to be used for transmission
 */
void light_ook_config(const int gpio_num);
void light_pwm_config();


/**
 * @brief Transmit OOK data
 * @param data: char pointer to data to be transmitted
 * @param gpio_num: GPIO number of the pin to be used for transmission
 */
void transmit_ook(const char *data, const int gpio_num);


void transmit_ascii(const char* data, const int gpio_num);

#endif