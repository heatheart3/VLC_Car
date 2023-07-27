#include "./include/light.h"
#include "./include/mytimer.h"
#include "./include/constant.h"

void light_pwm_config()
{
    ledc_timer_config_t timer_con = {};
    timer_con.duty_resolution = LEDC_TIMER_2_BIT; // resolution =  (100 / 8192) %
    timer_con.freq_hz = 20 * 1000;
    timer_con.speed_mode = LEDC_HIGH_SPEED_MODE;
    timer_con.timer_num = LEDC_TIMER_1;
    ledc_timer_config(&timer_con);

    ledc_channel_config_t channel_con = {};
    channel_con.channel = LEDC_CHANNEL_1;
    channel_con.gpio_num = GPIO_LEFT_LIGHT;
    channel_con.duty = 2;
    channel_con.speed_mode = LEDC_HIGH_SPEED_MODE;
    channel_con.timer_sel = LEDC_TIMER_1;
    ledc_channel_config(&channel_con);
}

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

void transmit_ook(const char *data, int GPIO_OUTPUT_LIGHT)
{
    // printf("transmit_ook: %s\n",data);
    for (int i = 0; i < strlen(data); i++)
    {
        if (data[i] == '1')
            gpio_set_level(GPIO_OUTPUT_LIGHT, 1);
        else
            gpio_set_level(GPIO_OUTPUT_LIGHT, 0);
        ets_delay_us(TRANSMIT_PERIOD);
    }
}

void transmit_ascii(const char *data, int GPIO_OUTPUT_LIGHT)
{
    volatile char bit;
    transmit_ook(FRAME_HEADER, GPIO_RIGHT_LIGHT);
    for (int i = 0; i < strlen(data); i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            bit = (data[i] >> j) & 0x01;
            if (bit == 1)
            {
                gpio_set_level(GPIO_OUTPUT_LIGHT, 0);
                ets_delay_us(TRANSMIT_PERIOD);
                gpio_set_level(GPIO_OUTPUT_LIGHT, 1);
                ets_delay_us(TRANSMIT_PERIOD);

            }
            else
            {
                gpio_set_level(GPIO_OUTPUT_LIGHT, 1);
                ets_delay_us(TRANSMIT_PERIOD);
                gpio_set_level(GPIO_OUTPUT_LIGHT, 0);
                ets_delay_us(TRANSMIT_PERIOD);

            }
        }
    }
    gpio_set_level(GPIO_OUTPUT_LIGHT, 0);
    ets_delay_us(TRANSMIT_PERIOD);
}

void manchester_OOK(uint8_t *symbols, const int GPIO_OUTPUT_LIGHT)
{

    uint8_t transmit_tmp[PASS_LENGTH*C] = {0};
    uint8_t tmp_counter=0;

    for(int i=0;i<PASS_LENGTH;i++)
    {
        for(int j=C-1;j>=0;j--)
        {
            transmit_tmp[tmp_counter++] = (symbols[i] >> j) & 0x01;
        }
    }
    free(symbols);
    // 1. preamble: 011110
    transmit_ook(FRAME_HEADER, GPIO_RIGHT_LIGHT);

    // 2.manchester encoding then OOK modulation
    // for (int i = 0; i < PASS_LENGTH; i++)
    // {
    //     for (int j = C - 1; j >= 0; j--)
    //     {
    //         bit = (symbols[i] >> j) & 0x01;

    //         // 1->01
    //         if (bit == 1)
    //         {
    //             gpio_set_level(GPIO_OUTPUT_LIGHT, 0);
    //             ets_delay_us(TRANSMIT_PERIOD);
    //             gpio_set_level(GPIO_OUTPUT_LIGHT, 1);
    //             ets_delay_us(TRANSMIT_PERIOD);
    //         }

    //         // 0->10
    //         else
    //         {
    //             gpio_set_level(GPIO_OUTPUT_LIGHT, 1);
    //             ets_delay_us(TRANSMIT_PERIOD);
    //             gpio_set_level(GPIO_OUTPUT_LIGHT, 0);
    //             ets_delay_us(TRANSMIT_PERIOD);
    //         }
    //     }
    // }

    for(int i=0;i<PASS_LENGTH*C;i++)
    {
        if(transmit_tmp[i]==1)
        {
            gpio_set_level(GPIO_OUTPUT_LIGHT, 0);
            ets_delay_us(TRANSMIT_PERIOD);
            gpio_set_level(GPIO_OUTPUT_LIGHT, 1);
            ets_delay_us(TRANSMIT_PERIOD);
        }
        else
        {
            gpio_set_level(GPIO_OUTPUT_LIGHT, 1);
            ets_delay_us(TRANSMIT_PERIOD);
            gpio_set_level(GPIO_OUTPUT_LIGHT, 0);
            ets_delay_us(TRANSMIT_PERIOD);
        }
    }
    // 3. postamble: 0
    gpio_set_level(GPIO_OUTPUT_LIGHT, 0);
    ets_delay_us(TRANSMIT_PERIOD);
}


void transmit_8bitz(const uint8_t data, const int GPIO_OUTPUT_LIGHT)
{
    volatile uint8_t bit=0;
        for (int j = 7; j >= 0; j--)
    {
        bit = (data >> j) & 0x01;
        if (bit == 1)
        {
            gpio_set_level(GPIO_OUTPUT_LIGHT, 0);
            ets_delay_us(TRANSMIT_PERIOD);
            gpio_set_level(GPIO_OUTPUT_LIGHT, 1);
            ets_delay_us(TRANSMIT_PERIOD);
        }
        else
        {
            gpio_set_level(GPIO_OUTPUT_LIGHT, 1);
            ets_delay_us(TRANSMIT_PERIOD);
            gpio_set_level(GPIO_OUTPUT_LIGHT, 0);
            ets_delay_us(TRANSMIT_PERIOD);
        }
    }
}