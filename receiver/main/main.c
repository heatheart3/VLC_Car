#include "./include/mytimer.h"
#include "./include/decoder.h"
#include "./include/vlc_esp32_hardware.h"
#include "./include/test.h"

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/stream_buffer.h"


#define PD_GPIO_NUM 35
#define SYMBOLS_BUFFER_SIZE 6000

uint8_t overflow_symbol_counter=0;
uint8_t decode_right_conuter=0;
int pass_counter=0;

// static uint8_t symbols_buffer[SYMBOLS_BUFFER_SIZE];


uint64_t s_count=0;
uint64_t e_count=0;
uint64_t s_error_count=0;
gptimer_handle_t gptimer =NULL;

StreamBufferHandle_t StreamBuffer = NULL;
bool timer_isr_flag=false;
void vtask_read(void *ptParam)
{

    static uint8_t send_buffer[SYMBOLS_BUFFER_SIZE];
    int send_bytes=0;
    while(1)
    {
        PHY_read_symbols(send_buffer,SYMBOLS_BUFFER_SIZE,PD_GPIO_NUM);
        send_bytes= xStreamBufferSend(StreamBuffer, 
                            (void*)send_buffer, 
                            SYMBOLS_BUFFER_SIZE, 
                            0);

        if(send_bytes==0)
        {
            printf("-1\n");
        }
        // printf("-------------\n");
        // printf("send_bytes:%d\n",send_bytes);
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void vtask_operate(void *ptParam)
{

    static uint8_t symbols_buffer[SYMBOLS_BUFFER_SIZE];
    int recv_bytes=0;
    uint32_t Rcounter=0;
    uint32_t tmp=0;
    while(1)
    {
        if(timer_isr_flag)
        {
            printf("counter:%ld\n",Rcounter);
            timer_isr_flag=false;
            Rcounter=0;
        }
        recv_bytes=xStreamBufferReceive(StreamBuffer, 
                                (void*)symbols_buffer, 
                                sizeof(symbols_buffer), 
                                0);

            
        tmp=test0_get_packet_ver2(symbols_buffer, SYMBOLS_BUFFER_SIZE);
        Rcounter+=tmp;
        // test_print_PHY_symbols_buffer(symbols_buffer, SYMBOLS_BUFFER_SIZE);
        // printf("-----------------\n");
        // printf("recv_bytes:%d\n",recv_bytes);
        // vTaskDelay(3000 / portTICK_PERIOD_MS);

    }
}


void app_main(void)
{
    // timer_clock_init(&gptimer);
    my_timer_init(&gptimer);
    ESP_ERROR_CHECK(gptimer_start(gptimer));
    PHY_gpio_config(PD_GPIO_NUM);

    uint8_t symbolsB[]={1,10,4,8,1,2,2,8,6,14,11};

    // while(1)
    // {
    //     PHY_read_symbols(symbols_buffer, SYMBOLS_BUFFER_SIZE, PD_GPIO_NUM);
    //     // test_print_PHY_symbols_buffer(symbols_buffer, SYMBOLS_BUFFER_SIZE);


    //     // test0_get_packet_ver2(symbols_buffer, SYMBOLS_BUFFER_SIZE);
    //     // test1_get_packet_spinal(symbols_buffer, SYMBOLS_BUFFER_SIZE);
    //     test1_get_packet_spinal_ver2(symbols_buffer, SYMBOLS_BUFFER_SIZE);   
    //     // test2_get_overall_latency(symbols_buffer, SYMBOLS_BUFFER_SIZE,symbolsB);
    // }

    StreamBuffer= xStreamBufferCreate(SYMBOLS_BUFFER_SIZE*5,SYMBOLS_BUFFER_SIZE);

    if(StreamBuffer !=NULL)
    {
    xTaskCreatePinnedToCore(vtask_read, "vtask_read", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(vtask_operate, "vtask_operate", 2048, NULL, 1, NULL, 1);
    }
    else
    {
        printf("StreamBuffer create failed\n");
    }
}
