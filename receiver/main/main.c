#include "./include/light.h"
#include "./include/mytimer.h"
#include "./include/myadc.h"
#include "./include/motor.h"
#include "./include/servo.h"
#include "./include/sensor.h"
#include "./include/decoder.h"

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define VLT_HIGH 2700
#define VLT_LOW 0
#define DECODED_MES_LEN 3
#define VALID_MES_LEN 6
char decode_data[DECODED_MES_LEN] = {0};
char mes_buffer[20] = {0};

static adc_channel_t channel[1] = {ADC_CHANNEL_7}; // gpio35
int process_count = 0;

void caculate_process_speed(void *parameter)
{
    while (1)
    {
        // printf("process speed : %d\n", process_count);
        process_count = 0;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
}

void app_main(void)
{

    // motor_config();

    // servo_config();

    // turn_forward();

    /*variable for vlc*/


    esp_err_t ret;
    uint32_t ret_num = 0;
    uint8_t result[EXAMPLE_READ_LEN] = {0};
    //sensor mark: used to indicate light sensor logic status
    uint8_t sensor_mask = 0, sensor_start_mark = 0;
    //using slope to sensor
    uint16_t  last_vlt = 0;
    uint8_t decode_data_result[200];
    adc_continuous_handle_t handle = NULL;

    memset(result, 0xcc, EXAMPLE_READ_LEN);
    continuous_adc_init(channel, sizeof(channel) / sizeof(adc_channel_t), &handle);
    ESP_ERROR_CHECK(adc_continuous_start(handle));
    xTaskCreate(caculate_process_speed, "caculate speed", 4096, NULL, 3 , NULL);

    while (1)
    {
        ret = adc_continuous_read(handle, result, EXAMPLE_READ_LEN, &ret_num, 0);
        if (ret == ESP_OK)
        {
            // printf("value:-4000\n");
            ESP_LOGI("TASK", "ret is %x, ret_num is %"PRIu32, ret, ret_num);

            uint16_t mes_num;
            memset(mes_buffer, 0, sizeof(mes_buffer));
            memset(decode_data, 0, sizeof(decode_data));
            decode(result, ret_num, decode_data_result, &mes_num);
            process_count += ret_num / SOC_ADC_DIGI_RESULT_BYTES;
            // for(int i = 0; i < mes_num; i++)
            // {
            //     printf("Value:%d\n", decode_data_result[i]);
            // }
            // printf("mes_num: %d\n", mes_num);

                    // /***sensor part: judege quality of communication***/
                    // sensor_mask = sensor_by_slope(last_vlt, tmp_vlt);
                    
                    //     //only when signal is changed from square waves to irregular wave, sensoring makes sense.
                    //     //so judge this condition first
                    // if(sensor_start_mark != 0)
                    // {
                    //     if(sensor_mask == 1 )
                    //     {
                    //         break;
                    //     }
                    // }
                    // else
                    // {
                    //     //first detected a square wave, then set sensor_start_mark to 1
                    //     if(sensor_mask == 0)
                    //     {
                    //         sensor_start_mark = 1;
                    //     }
                    // }
                    // /***sensor part end***/

                   
            }
            //the front car's motion has changed
            if(sensor_mask == 1)
            {
                break;
            }
    }

    ESP_ERROR_CHECK(adc_continuous_stop(handle));
    ESP_ERROR_CHECK(adc_continuous_deinit(handle));
}
