
/*
 * @brief: This file contains the main program for the LED
 *         It will blink the LED and transmit the OOK signal
 *         to the receiver
 *
 */

#include "./include/light.h"
#include "./include/mytimer.h"
#include "./include/encoder.h"
#include "./include/test.h"

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <sys/time.h>



#define BYTE8 "IloveSCU"



#define BYTE50  "abcdefghijklmnopqrstuvwxyz123,abcdefghijklmnopqrst"
#define BYTE100 "abcdefghijklmnopqrstuvwxyz123,abcdefghijklmnopqrstuvwxyz123,abcdefghijklmnopqrstuvwxyz123,0123456789"
#define BYTE204 "abcdefghijklmnopqrstuvwxyzXabcdefghijklmnopqrstuvwxyzXabcdefghijklmnabcdefghijklmnopqrstuvwxyzXabcdefghijklmnopqrstuvwxyzXabcdefghijklmnabcdefghijklmnopqrstuvwxyzXabcdefghijklmnopqrstuvwxyzXabcdefghijklmn"

void app_main(void)
{

    light_ook_config(GPIO_RIGHT_LIGHT);
    gpio_set_level(GPIO_RIGHT_LIGHT, 1);
    while (1)
    {
        // test_square_wave(); 
        // test_10_1100();

        // test_transmit_raptor();
        test_transmit_original(BYTE204);
    }
}