
/*
 * @brief: This file contains the main program for the LED
 *         It will blink the LED and transmit the OOK signal
 *         to the receiver
 *
 */

#include "./include/light.h"
#include "./include/mytimer.h"
#include "./include/encoder.h"

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp32/rom/ets_sys.h"
#include <sys/time.h>

// transmit "0!AaA]&" in a loop
void loop_transmit_ascii()
{
    uint16_t cnt = 0;
    uint8_t mask = 0, temp = 0;
    const uint32_t interval = 100;
    const uint32_t test_period = 7 * interval;
    while (cnt <= test_period + 1)
    {
        if (cnt % interval == 0)
        {
            temp = cnt / interval;
            switch (temp)
            {
            case 1:
                // printf("0\n");
                transmit_ascii("0", GPIO_RIGHT_LIGHT);
                break;
            // case 1:
            case 3:
            case 5:
                //    printf("A\n");
                transmit_ascii("A", GPIO_RIGHT_LIGHT);
                break;
            case 2:
                // printf("!\n");
                transmit_ascii("!", GPIO_RIGHT_LIGHT);
                break;
            case 4:
                // printf("a\n");
                transmit_ascii("a", GPIO_RIGHT_LIGHT);
                break;
            case 6:
                // printf("]\n");
                transmit_ascii("]", GPIO_RIGHT_LIGHT);
                break;
            case 7:
                // printf("&\n");
                transmit_ascii("&", GPIO_RIGHT_LIGHT);
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

// test udelay() and  gpio_set_level() time consumption
void test_time_consumption_01()
{
    struct timeval tv_now;
    int64_t time_us_before, time_us_after;
    while (1)
    {
        gettimeofday(&tv_now, NULL);
        time_us_before = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        gpio_set_level(GPIO_RIGHT_LIGHT, 1);
        gettimeofday(&tv_now, NULL);
        time_us_after = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        printf("Set gpio 1=%lld\n", time_us_after - time_us_before);

        gettimeofday(&tv_now, NULL);
        time_us_before = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        udelay(TRANSMIT_PERIOD);
        gettimeofday(&tv_now, NULL);
        time_us_after = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        printf("udelay=%lld\n", time_us_after - time_us_before);

        gettimeofday(&tv_now, NULL);
        time_us_before = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        gpio_set_level(GPIO_RIGHT_LIGHT, 0);
        gettimeofday(&tv_now, NULL);
        time_us_after = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        printf("Set gpio 0=%lld\n", time_us_after - time_us_before);

        gettimeofday(&tv_now, NULL);
        time_us_before = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        udelay(TRANSMIT_PERIOD);
        gettimeofday(&tv_now, NULL);
        time_us_after = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        printf("udelay=%lld\n", time_us_after - time_us_before);

        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

// test all endoing shceme in one function
void test_allinone()
{
    char *mesA = "abcd";
    char *mesB = "XXUU";
    while (1)
    {
        printf("Transmitting message B: %s\n", mesB);
        uint8_t *symbols = SpinalEncode(mesA);
        uint8_t *test_symbols = SpinalEncode(mesB);
        uint8_t *network_code = network_coding(symbols, test_symbols);

        manchester_OOK(network_code, GPIO_RIGHT_LIGHT);

        for (int i = 0; i < 10000; i++)
        {
            gpio_set_level(GPIO_RIGHT_LIGHT, 1);
            udelay(TRANSMIT_PERIOD);
            gpio_set_level(GPIO_RIGHT_LIGHT, 0);
            udelay(TRANSMIT_PERIOD);
        }
    }
}

void app_main(void)
{

    light_ook_config(GPIO_RIGHT_LIGHT);
    gpio_set_level(GPIO_RIGHT_LIGHT, 1);

    gptimer_handle_t timer = NULL;
    my_timer_init(&timer);
    while (1)
    {
        // test_allinone();

            gpio_set_level(GPIO_RIGHT_LIGHT, 1);
            ets_delay_us(5);
            gpio_set_level(GPIO_RIGHT_LIGHT, 0);
            ets_delay_us(5);

            gpio_set_level(GPIO_RIGHT_LIGHT, 1);
            ets_delay_us(5);
            gpio_set_level(GPIO_RIGHT_LIGHT, 1);
            ets_delay_us(5);
            gpio_set_level(GPIO_RIGHT_LIGHT, 0);
            ets_delay_us(5);
            gpio_set_level(GPIO_RIGHT_LIGHT, 0);
            ets_delay_us(5);
    }
}