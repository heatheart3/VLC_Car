
/*
 * @brief: This file contains the main program for the LED
 *         It will blink the LED and transmit the OOK signal
 *         to the receiver
 *
 */

#include "light.h"
#include "./include/test.h"

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"


#define GPIO_LIGHT 27

#define BYTE4 "abcd"
#define BYTE8 "abcddcba"
#define BYTE50  "abcdefghijklmnopqrstuvwxyz123,abcdefghijklmnopqrst"
#define BYTE100 "abcdefghijklmnopqrstuvwxyz123,abcdefghijklmnopqrstuvwxyz123,abcdefghijklmnopqrstuvwxyz123,0123456789"
#define BYTE204 "abcdefghijklmnopqrstuvwxyzXabcdefghijklmnopqrstuvwxyzXabcdefghijklmnabcdefghijklmnopqrstuvwxyzXabcdefghijklmnopqrstuvwxyzXabcdefghijklmnabcdefghijklmnopqrstuvwxyzXabcdefghijklmnopqrstuvwxyzXabcdefghijklmn"

void app_main(void)
{

    light_ook_config(GPIO_LIGHT);
    test_config(GPIO_LIGHT);

    while(1)
    {
        test_transmit_original(BYTE8);
        // light_transmit_frame(BYTE4, 0, GPIO_LIGHT);
    }
}