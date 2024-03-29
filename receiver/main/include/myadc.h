#ifndef __MYADC_H__
#define __MYADC_H__

#include "esp_adc/adc_continuous.h"


#define EXAMPLE_READ_LEN 4000  //ADC conversion buffer length
#define EXAMPLE_ADC_CONV_MODE ADC_CONV_SINGLE_UNIT_1


#define EXAMPLE_ADC_USE_OUTPUT_TYPE1 1
#define EXAMPLE_ADC_OUTPUT_TYPE ADC_DIGI_OUTPUT_FORMAT_TYPE1

extern int overflow_mark;


/**
 * @brief Check if the data is valid
 * @param data: pointer to the data
 * @return true if valid, false if invalid
 */
bool check_valid_data(const adc_digi_output_data_t *data);


/**
 * @brief Initialize continuous ADC
 * @param channel: pointer to the channel array
 * @param channel_num: number of channels
 * @param out_handle: pointer to the adc handle
 */
void continuous_adc_init(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle);

void detect_overflow(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data);

#endif