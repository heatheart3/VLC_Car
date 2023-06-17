#include "./include/light.h"
#include "./include/mytimer.h"
#include "./include/myadc.h"
#include "./include/motor.h"
#include "./include/servo.h"
#include "./include/sensor.h"
#include "./include/decoder.h"
#include "./include/test.h"

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define DECODED_MES_LEN 3
#define VALID_MES_LEN 6
char decode_data[DECODED_MES_LEN] = {0};
char mes_buffer[20] = {0};

static adc_channel_t channel[1] = {ADC_CHANNEL_7}; // gpio35
int process_count = 0;
int overflow_mark = 0;
uint16_t collected_raw_data[5000];





void app_main(void)
{

    // motor_config();

    // servo_config();

    // turn_forward();

    esp_err_t ret;
    uint32_t ret_num = 0;
    uint8_t result[EXAMPLE_READ_LEN] = {0};
    //sensor mark: used to indicate light sensor logic status
    uint8_t sensor_mask = 0, sensor_start_mark = 0;
    //using slope to sensor
    uint8_t decode_data_result[200];
    adc_continuous_handle_t conti_adc_handle = NULL;
    adc_continuous_evt_cbs_t adc1_handle;
    bool collected_over_mark = false;

    adc1_handle.on_pool_ovf = detect_overflow;
    memset(result, 0xcc, EXAMPLE_READ_LEN);
    memset(collected_raw_data, 0x00, sizeof(collected_raw_data));
    continuous_adc_init(channel, sizeof(channel) / sizeof(adc_channel_t), &conti_adc_handle);
    ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(conti_adc_handle, &adc1_handle, NULL));
    ESP_ERROR_CHECK(adc_continuous_start(conti_adc_handle));
    // xTaskCreate(caculate_process_speed, "caculate speed", 4096, NULL, 3 , NULL);
    

    while (1)
    {

        ret = adc_continuous_read(conti_adc_handle, result, EXAMPLE_READ_LEN, &ret_num, 10);
        // ESP_LOGI("TASK", "ret is %x, ret_num is %"PRIu32, ret, ret_num);
        //可视化调试部分
        collected_over_mark = collect_data(result,ret_num,collected_raw_data,sizeof(collected_raw_data)/sizeof(uint16_t));
        if(collected_over_mark)
        {
            ESP_ERROR_CHECK(adc_continuous_stop(conti_adc_handle));
            for(uint16_t i = 0; i < (sizeof(collected_raw_data)/sizeof(uint16_t)); i++)
            {
                printf("value:%d\n", collected_raw_data[i]);
            }
            ESP_ERROR_CHECK(adc_continuous_start(conti_adc_handle));
        }
        
        if (ret == ESP_OK)
        {
            ESP_ERROR_CHECK(adc_continuous_stop(conti_adc_handle));
            uint16_t mes_num;
            memset(mes_buffer, 0, sizeof(mes_buffer));
            memset(decode_data, 0, sizeof(decode_data));
            // decoder(result, ret_num, decode_data_result, &mes_num);
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

    ESP_ERROR_CHECK(adc_continuous_deinit(conti_adc_handle));
}
