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
#include "./include/encoder.h"

// A0 32
// A1 33
// B6 22
// B7 23
extern int encoder_edge_cnt;
extern int data[200];
 

void app_main(void)
{
    light_ook_config();
    motor_config();
    servo_config();
    encoder_config();
    encoder_timer_config();
    // turn_left45();
    // turn_forward();
    
    // vTaskDelay(300);
    // stop();
    turn_forward();
    forward(20);
    
    while (1)
    {

    }
    

}

