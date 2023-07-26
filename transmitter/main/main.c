
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
#define BYTE16 "IloveSCUSCUloveI"
#define BYTE32 "IloveSCUSCUloveI12345678HAHAsu~~"
#define BYTE64 "IloveSCUSCUloveI12345678HAHAsu~~8848royal!!brandnew!!xdflopo;].;"
#define BYTE128 "IloveSCUSCUloveI12345678HAHAsu~~8848royal!!brandnew!!xdflopo;].;IloveSCUSCUloveI12345678HAHAsu~~8848royal!!brandnew!!xdflopo;].;"
#define BYTE256
#define BYTE512

void app_main(void)
{

    light_ook_config(GPIO_RIGHT_LIGHT);
    gpio_set_level(GPIO_RIGHT_LIGHT, 1);
    while (1)
    {
        // test_get_header();
        // test_square_wave(); 
        // test_10_1100();
        // test0_transmit_nonencode_ver2("IloveSCU");
        // test0_transmit_nonencode_ver4("IloveSCUSCUlovesme123987howaboutvehicletalk!it'sreallycooldon92outhinkis912889joixzcvjxocjv98xcvxjcivoj0v9cxzjvxzcv9xzvxcjv9xc02");

        test0_transmit_nonencode_ver2(BYTE8);
        // 8 bytes
        // test0_transmit_nonencode_ver4("IloveSCU");

        // 16 bytes
        // test0_transmit_nonencode_ver4("IloveSCUSCUloveI");
        
        // 32 bytes
        // test0_transmit_nonencode_ver4("IloveSCUSCUloveI12345678HAHAsu~~");

        // test0_transmit_nonencode_ver4(BYTE128);
        // test1_transmit_spinal_ver2("IloveSCU");
        // test1_transmit_spinal_ver3("IloveSCU");
        // test1_transmit_spinal_ver4(BYTE8);
    //     // test2_transmit_allinone();


        // test_transmit_raptor();
    }
}