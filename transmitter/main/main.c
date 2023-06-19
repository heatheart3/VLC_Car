
/*
 * @brief: This file contains the main program for the LED
 *         It will blink the LED and transmit the OOK signal
 *         to the receiver
 *
 */

#include "./include/light.h"
#include "./include/mytimer.h"
#include "./include/myadc.h"
#include "./include/motor.h"
#include "./include/servo.h"

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define OOK_STOP "011100101100"     //110
#define OOK_FORWARD "011100110010" //101
#define OOK_BACKWARD "011101001100" //010
#define OOK_LEFT "011101010010"     //001
#define OOK_RIGHT "011100110100"    //100

#define STATE_NUM 7


void app_main(void)
{

    // Initialize config
    light_ook_config(GPIO_RIGHT_LIGHT);
    motor_config();
    servo_config();
    gpio_set_level(GPIO_RIGHT_LIGHT, 1);

    uint16_t cnt = 0;
    uint8_t mask = 0, temp = 0;
    const uint8_t state_amount = STATE_NUM;
    const uint32_t interval = 100;
    const uint32_t test_period = state_amount * interval;

    enum CAR_STATE current_state= ENUM_STOP;

    while (1)
    {
        cnt = 0;
        while (cnt <= test_period + 1)
        {
            if (cnt % interval == 0)
            {
                temp = cnt / interval;
                switch (temp)
                {
                case 1:
                    // printf("0\n");
                    transmit_ascii("0",GPIO_RIGHT_LIGHT);
                    break;
                // case 1:
                case 3:
                case 5:
                //    current_state = ENUM_FORWARD;
                //    transmit_ook(OOK_FORWARD,GPIO_RIGHT_LIGHT);
                //    forward(24);
                //    printf("A\n");
                    transmit_ascii("A",GPIO_RIGHT_LIGHT);
                    break;
                case 2:
                    // current_state = ENUM_LEFT;
                    // transmit_ook(OOK_FORWARD,GPIO_RIGHT_LIGHT);
                    // forward(24);
                    // printf("!\n");
                    transmit_ascii("!",GPIO_RIGHT_LIGHT);
                    break;
                case 4:
                    // current_state = ENUM_RIGHT;
                    // transmit_ook(OOK_BACKWARD,GPIO_RIGHT_LIGHT);
                    // backward(24);
                    // printf("a\n"); 
                    transmit_ascii("a",GPIO_RIGHT_LIGHT);
                    break;
                case 6:
                    // current_state = ENUM_BACKWARD;
                    // transmit_ook(OOK_FORWARD,GPIO_RIGHT_LIGHT);
                    // forward(24);
                    // printf("]\n");
                    transmit_ascii("]",GPIO_RIGHT_LIGHT);
                    break;
                case 7:
                    // current_state = ENUM_STOP;
                    // transmit_ook(OOK_STOP,GPIO_RIGHT_LIGHT);
                    // stop();
                    // printf("&\n");
                    transmit_ascii("&",GPIO_RIGHT_LIGHT);
                    break;
                }
            }
            else
            {
                gpio_set_level(GPIO_RIGHT_LIGHT, mask);
                mask = 1 - mask;
            }
            cnt++;
            udelay(TRANSMIT_PERIOD);
        }
    }
}