#include "./include/decoder.h"
#include "./include/vlc_esp32_hardware.h"
#include "./include/test.h"

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


#define PD_GPIO_NUM 35
#define SYMBOLS_BUFFER_SIZE 3000


static uint8_t symbols_buffer[SYMBOLS_BUFFER_SIZE];


void app_main(void)
{
    PHY_gpio_config(PD_GPIO_NUM);
    while(1)
    {
        PHY_read_symbols(symbols_buffer, SYMBOLS_BUFFER_SIZE, PD_GPIO_NUM);
        // print_PHY_symbols_buffer(symbols_buffer, SYMBOLS_BUFFER_SIZE);


        uint8_t mes_buffer[OOK_SYMBOLS_LEN];
        uint8_t manchester_symbols[MANCHESTER_SYMBOLS_LEN];
        for(uint16_t i=0;i<SYMBOLS_BUFFER_SIZE;i++)
        {
            PHY_demoluate_OOK(symbols_buffer, &i, SYMBOLS_BUFFER_SIZE, mes_buffer);
            PHY_decode_manchester(mes_buffer,manchester_symbols);
            for(int i=0;i<MANCHESTER_SYMBOLS_LEN;i++)
            {
                printf("%d",manchester_symbols[i]);
            }
            printf("\n");
        }
    }
}
