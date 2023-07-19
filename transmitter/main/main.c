
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


void vtask_transmitter()
{
        while (1)
    {
        
        // test_square_wave(); 
        // test_10_1100();
        test0_transmit_nonencode_ver2("Fo1!");
        // test1_transmit_spinal("Fo1!");
        // test1_transmit_spinal_ver2("Fo1!");
        // test2_transmit_allinone();
    }
}

void vtask_test()
{
    int i=0;
    while(1)
    {
        i++;
        i++;
    }
}
void app_main(void)
{

    light_ook_config(GPIO_RIGHT_LIGHT);
    gpio_set_level(GPIO_RIGHT_LIGHT, 1);

    TaskHandle_t Handle1 =NULL;
    TaskHandle_t Handle2 =NULL;

    xTaskCreatePinnedToCore(vtask_transmitter,"transmitter",8192,NULL,1,&Handle1,0);
    xTaskCreatePinnedToCore(vtask_test,"test",2048,NULL,1,&Handle2,1);

    // while (1)
    // {
        
    //     test_square_wave(); 
    //     // test_10_1100();
    //     // test0_transmit_nonencode_ver2("Fo1!");
    //     // test1_transmit_spinal("Fo1!");
    //     // test1_transmit_spinal_ver2("Fo1!");
    //     // test2_transmit_allinone();
    // }
}