#include "./include/mytimer.h"
#include "./include/decoder.h"
#include "./include/vlc_esp32_hardware.h"
#include "./include/test.h"

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/message_buffer.h"


#define PD_GPIO_NUM 35
#define SYMBOLS_BUFFER_SIZE 6000
#define SYMBOLS_BUFFER_SIZE2 2000

uint8_t overflow_symbol_counter=0;
uint8_t decode_right_conuter=0;


uint64_t s_count=0;
uint64_t e_count=0;
uint64_t s_error_count=0;
gptimer_handle_t gptimer =NULL;


MessageBufferHandle_t MessageBuffer = NULL;
bool timer_isr_flag=false;
void vtask_read(void *ptParam)
{

    uint8_t send_buffer[SYMBOLS_BUFFER_SIZE2];
    int send_bytes=0;
    while(1)
    {
        memset(send_buffer,0,SYMBOLS_BUFFER_SIZE2*sizeof(uint8_t));
        PHY_read_symbols(send_buffer,SYMBOLS_BUFFER_SIZE2,PD_GPIO_NUM);
        send_bytes= xMessageBufferSend(MessageBuffer, 
                            (void*)send_buffer, 
                            SYMBOLS_BUFFER_SIZE2, 
                            0);

        if(send_bytes==0)
        {
            xMessageBufferReset(MessageBuffer);
            printf("-2\n");
        }
        // printf("-------------\n");
        // printf("send_bytes:%d\n",send_bytes);
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void vtask_operate(void *ptParam)
{

    uint8_t symbols_buffer[SYMBOLS_BUFFER_SIZE2];
    int recv_bytes=0;
    uint32_t Rcounter=0;
    uint32_t tmp=0;
    while(1)
    {
        // if(timer_isr_flag)
        // {
        //     printf("%ld\n",Rcounter);
        //     timer_isr_flag=false;
        //     Rcounter=0;
        // }

        memset(symbols_buffer,0,SYMBOLS_BUFFER_SIZE2*sizeof(uint8_t));
        recv_bytes=xMessageBufferReceive(MessageBuffer, 
                                (void*)symbols_buffer, 
                                sizeof(symbols_buffer), 
                                portMAX_DELAY);

        // for(int i=0;i<SYMBOLS_BUFFER_SIZE2;i++)
        // {
        //     printf("%d\n",symbols_buffer[i]);
        // }
        // printf("-1\n");

        // test_get_raptor(symbols_buffer,SYMBOLS_BUFFER_SIZE2);

        // tmp=test0_get_packet_ver3(symbols_buffer, SYMBOLS_BUFFER_SIZE2);
        test0_get_packet_ver2(symbols_buffer, SYMBOLS_BUFFER_SIZE2);
        // test1_get_packet_spinal(symbols_buffer,SYMBOLS_BUFFER_SIZE2);
        // test1_get_packet_spinal_ver3(symbols_buffer,SYMBOLS_BUFFER_SIZE2);
        // Rcounter+=tmp;
        // test_print_PHY_symbols_buffer(symbols_buffer, SYMBOLS_BUFFER_SIZE);
        // printf("-----------------\n");
        // printf("recv_bytes:%d\n",recv_bytes);
        // vTaskDelay(3000 / portTICK_PERIOD_MS);

    }
}


void app_main(void)
{
    timer_clock_init(&gptimer);
    // my_timer_init(&gptimer);
    ESP_ERROR_CHECK(gptimer_start(gptimer));
    PHY_gpio_config(PD_GPIO_NUM);



    // uint8_t test_buffer[SYMBOLS_BUFFER_SIZE];
    // while(1)
    // {
    //     PHY_easy_read_symbols(test_buffer,SYMBOLS_BUFFER_SIZE,PD_GPIO_NUM);
    //     test_print_PHY_symbols_buffer(test_buffer, SYMBOLS_BUFFER_SIZE);


    //     // test0_get_packet_ver2(symbols_buffer, SYMBOLS_BUFFER_SIZE);
    //     // test1_get_packet_spinal(symbols_buffer, SYMBOLS_BUFFER_SIZE);
    //     // test1_get_packet_spinal_ver2(symbols_buffer, SYMBOLS_BUFFER_SIZE);   
    //     // test2_get_overall_latency(symbols_buffer, SYMBOLS_BUFFER_SIZE,symbolsB);
    // }

    MessageBuffer= xMessageBufferCreate(SYMBOLS_BUFFER_SIZE*10);

    if(MessageBuffer !=NULL)
    {
    xTaskCreatePinnedToCore(vtask_read, "vtask_read", 8192, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(vtask_operate, "vtask_operate", 8192, NULL, 1, NULL, 1);
    }
    else
    {
        printf("MessageBuffer create failed\n");
    }
}
