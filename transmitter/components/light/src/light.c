#include "light.h"

void light_ook_config(const int gpio_num)
{
    gpio_config_t io_config = {
        .pin_bit_mask = (1 << gpio_num),
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_down_en = 0,
        .pull_up_en = 0};

    gpio_config(&io_config);
}

void light_transmit_frame(const char* payload, const uint8_t PSN, const int gpio_num)
{
    //1. transmit the Header of the frame
    transmit_ook(FRAME_HEADER, gpio_num);

    //2. transmit the PSN
    transmit_8bitz(PSN, gpio_num);

    //3. transmit the payload
    transmit_ascii(payload, gpio_num);

    //4. transmit the stop bit
    gpio_set_level(gpio_num, 0);
    ets_delay_us(SIGNAL_DURATION);
    ets_delay_us(SIGNAL_DURATION);  
    
}

void transmit_ook(const char *data, const int gpio_num)
{
    // printf("transmit_ook: %s\n",data);
    for (int i = 0; i < strlen(data); i++)
    {
        if (data[i] == '1')
            gpio_set_level(gpio_num, 1);
        else
            gpio_set_level(gpio_num, 0);
        ets_delay_us(SIGNAL_DURATION);
    }
}

void transmit_8bitz(const uint8_t data, const int gpio_num)
{
    volatile uint8_t bit=0;
    for (int j = 7; j >= 0; j--)
    {
        bit = (data >> j) & 0x01;
        if (bit == 1)
        {
            gpio_set_level(gpio_num, 0);
            ets_delay_us(SIGNAL_DURATION);
            gpio_set_level(gpio_num, 1);
            ets_delay_us(SIGNAL_DURATION);
        }
        else
        {
            gpio_set_level(gpio_num, 1);
            ets_delay_us(SIGNAL_DURATION);
            gpio_set_level(gpio_num, 0);
            ets_delay_us(SIGNAL_DURATION);
        }
    }
}

void transmit_ascii(const char *data, int gpio_num)
{
    volatile char bit;
    for (int i = 0; i < strlen(data); i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            bit = (data[i] >> j) & 0x01;
            if (bit == 1)
            {
                gpio_set_level(gpio_num, 0);
                ets_delay_us(SIGNAL_DURATION);
                gpio_set_level(gpio_num, 1);
                ets_delay_us(SIGNAL_DURATION);

            }
            else
            {
                gpio_set_level(gpio_num, 1);
                ets_delay_us(SIGNAL_DURATION);
                gpio_set_level(gpio_num, 0);
                ets_delay_us(SIGNAL_DURATION);
            }
        }
    }
}