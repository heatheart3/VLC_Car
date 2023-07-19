#include "./include/mytimer.h"
#include "./include/decoder.h"
#include "./include/vlc_esp32_hardware.h"
#include "./include/test.h"

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


#define PD_GPIO_NUM 35
#define SYMBOLS_BUFFER_SIZE 6000

uint8_t overflow_symbol_counter=0;
uint8_t decode_right_conuter=0;
int pass_counter=0;

static uint8_t symbols_buffer[SYMBOLS_BUFFER_SIZE];


uint64_t s_count=0;
uint64_t e_count=0;
uint64_t s_error_count=0;
gptimer_handle_t gptimer =NULL;
void app_main(void)
{
    timer_clock_init(&gptimer);
    ESP_ERROR_CHECK(gptimer_start(gptimer));
    PHY_gpio_config(PD_GPIO_NUM);

    uint8_t symbolsB[]={1,10,4,8,1,2,2,8,6,14,11};
    while(1)
    {
        PHY_read_symbols(symbols_buffer, SYMBOLS_BUFFER_SIZE, PD_GPIO_NUM);
        // test_print_PHY_symbols_buffer(symbols_buffer, SYMBOLS_BUFFER_SIZE);


        // test0_get_packet_ver2(symbols_buffer, SYMBOLS_BUFFER_SIZE);
        // test1_get_packet_spinal(symbols_buffer, SYMBOLS_BUFFER_SIZE);
        test1_get_packet_spinal_ver2(symbols_buffer, SYMBOLS_BUFFER_SIZE);   
        // test2_get_overall_latency(symbols_buffer, SYMBOLS_BUFFER_SIZE,symbolsB);
    }
}
