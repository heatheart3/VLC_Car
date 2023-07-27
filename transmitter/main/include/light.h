#ifndef __LIGHT__H__
#define __LIGHT__H__

#include "./driver/ledc.h"
#include "./driver/gpio.h"
#include "esp32/rom/ets_sys.h"
#include <string.h>

#define GPIO_LEFT_LIGHT 26
#define GPIO_RIGHT_LIGHT 27

#define  MES_HEADER "11110000"


// #define TRANSMIT_PERIOD 50  //20KHz
// #define TRANSMIT_PERIOD 25  //40KHz
// #define TRANSMIT_PERIOD 16  //60KHz
#define TRANSMIT_PERIOD 12  //80KHz
// #define TRANSMIT_PERIOD 10 // 100KHz not useful


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

/**
 * @brief Transmit ASCII datas
 * @param data: char pointer to data to be transmitted, original ascii data
 * @param gpio_num: GPIO number of the pin to be used for transmission
 */
void transmit_ascii(const char* data, const int gpio_num);


/**
 * @brief Transmit OOK data
 * @param data: char pointer to data to be transmitted, the symbols should be encoded by spinalcodes.
 * @param gpio_num: GPIO number of the pin to be used for transmission
 */
void manchester_OOK(uint8_t* symbols,const int gpio_num);


void transmit_8bitz(const uint8_t data, const int GPIO_OUTPUT_LIGHT);

#endif