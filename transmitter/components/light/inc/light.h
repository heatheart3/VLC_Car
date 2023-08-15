#ifndef __LIGHT__H__
#define __LIGHT__H__

#include "driver/gpio.h"
#include "esp32/rom/ets_sys.h"
#include <string.h>


// #define SIGNAL_DURATION 50  //20KHz
// #define SIGNAL_DURATION 25  //40KHz
// #define SIGNAL_DURATION 16  //60KHz
#define SIGNAL_DURATION 12  //80KHz
// #define SIGNAL_DURATION 10 // 100KHz not useful
#define FRAME_HEADER "11110000"


/**
 * @brief Configure GPIO for OOK transmission
 * @param gpio_num: GPIO number of the pin to be used for transmission
 */
void light_ook_config(const int gpio_num);


/**
 * @brief Transmit a frame.
 * @param payload: char pointer to data to be transmitted.
 * @param PSN: PSN number of the frame
 * @param gpio_num: GPIO number of the pin to be used for transmission
 */
void light_transmit_frame(const char* payload, const uint8_t PSN, const int gpio_num);


/**
 * @brief Transmit OOK data
 * @param data: char pointer to data to be transmitted
 * @param gpio_num: GPIO number of the pin to be used for transmission
 */
void transmit_ook(const char *data, const int gpio_num);

/**
 * @brief: Transmit 8 bits data, with manchester encoding.
 */
void transmit_8bitz(const uint8_t data, const int GPIO_OUTPUT_LIGHT);

/**
 * @brief Transmit ASCII datas
 * @param data: char pointer to data to be transmitted, original ascii data
 * @param gpio_num: GPIO number of the pin to be used for transmission
 */
void transmit_ascii(const char* data, const int gpio_num);

#endif