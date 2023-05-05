
/*
    * @brief: This file contains the main program for the LED
    *         It will blink the LED and transmit the OOK signal
    *         to the receiver
    *
*/

#include "./include/light.h"
#include "./include/mytimer.h"
#include "./include/myadc.h"

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define OOK_STOP "011110010110"
#define OOK_FORWARD "011110011001"
#define OOK_BACKWARD "011110100110"

void app_main(void)
{

    light_ook_config();


    int count = 0;
    while (1)
    {
        gpio_set_level(GPIO_OUTPUT_LIGHT, 1);
        udelay(1000);
        gpio_set_level(GPIO_OUTPUT_LIGHT, 0);
        udelay(1000);

        if (count == 400)
        {
            transmit_ook(OOK_STOP);
        }
        else if (count == 800)
        {
            transmit_ook(OOK_FORWARD);
        }
        else if (count == 1200)
        {
            transmit_ook(OOK_STOP);
        }
        else if (count == 1600)
        {
            transmit_ook(OOK_BACKWARD);
            count = 0;
        }
        count++;
    }
}
