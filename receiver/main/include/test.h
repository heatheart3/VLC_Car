#ifndef __TEST__
#define __TEST__
#include <string.h>
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "./include/myadc.h"


void caculate_process_speed(void *parameter);
void detect_overflow(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data);


#endif