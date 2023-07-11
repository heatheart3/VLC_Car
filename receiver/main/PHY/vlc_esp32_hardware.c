#include "./include/vlc_esp32_hardware.h"

void PHY_gpio_config(const int gpio_num)
{
    gpio_config_t io_conf={};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL<<gpio_num);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
}

void PHY_read_symbols(uint8_t *buffer, uint16_t length, const int PD_GPIO_NUM)
{
    for(int i = 0; i < length; i++)
    {
        buffer[i] = gpio_get_level(PD_GPIO_NUM);
        ets_delay_us(1);
    }
}