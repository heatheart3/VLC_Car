#include "./include/light.h"
#include  "./include/mytimer.h"

void light_pwm_config()
{
    ledc_timer_config_t timer_con = {};
    timer_con.duty_resolution = LEDC_TIMER_13_BIT; //resolution =  (100 / 8192) %
    timer_con.freq_hz = 5000;
    timer_con.speed_mode = LEDC_HIGH_SPEED_MODE;
    timer_con.timer_num = LEDC_TIMER_1;
    ledc_timer_config(&timer_con);
    
    ledc_channel_config_t channel_con = {};
    channel_con.channel = LEDC_CHANNEL_1;
    channel_con.gpio_num = 15;
    channel_con.duty = 4095;
    channel_con.speed_mode = LEDC_HIGH_SPEED_MODE;
    channel_con.timer_sel = LEDC_TIMER_1;
    ledc_channel_config(&channel_con);
}

void light_ook_config()
{
    gpio_config_t io_config={
        .pin_bit_mask = (1<<GPIO_OUTPUT_LIGHT),
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_down_en = 0,
        .pull_up_en=0
    };

    gpio_config(&io_config);
}

void transmit_ook(const char *data)
{
    //printf("transmit_ook: %s\n",data);
    for(int i=0;i<strlen(data);i++)
    {
        if(data[i]=='1')
        gpio_set_level(GPIO_OUTPUT_LIGHT,1);
        else
        gpio_set_level(GPIO_OUTPUT_LIGHT,0);
        udelay(1000);
    }
}