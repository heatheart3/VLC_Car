#include"./include/test.h"

extern int overflow_mark;
extern int process_count;



void caculate_process_speed(void *parameter)
{
    while (1)
    {
        printf("process speed : %d\n", process_count);
        process_count = 0;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
}

void detect_overflow(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data)
{
    overflow_mark = 1;
}