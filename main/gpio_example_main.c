/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "./include/light.h"
#include "./include/motor.h"
#include "./include/servo.h"

/**
 * Brief:
 * This test code shows how to configure gpio and how to use gpio interrupt.
 *
 */

 

void app_main(void)
{
    light_ook_config();
    motor_config();
    servo_config();


    turn_left45();
    turn_forward();
    forward(30);
    vTaskDelay(300);
    stop();

    uint8_t a = 1;
    while(1) {
            a *= 1;
    }
}

