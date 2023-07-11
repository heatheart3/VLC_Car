#include "./include/test.h"

void clean_data(uint8_t *data, int data_num, uint16_t *result_data)
{
    uint16_t temp_vlts[2];
    uint8_t double_mark = 0;
    for (int i = 0; i < data_num; i += SOC_ADC_DIGI_RESULT_BYTES)
    {
        adc_digi_output_data_t *p = (void *)&data[i];
        if (check_valid_data(p))
        {
            //edge condition
            if( (i+SOC_ADC_DIGI_RESULT_BYTES) == data_num && (double_mark + 1) != 2)
            {
                result_data[i/SOC_ADC_DIGI_RESULT_BYTES] = p->type1.data;
                break;
            }

            temp_vlts[double_mark++] = p->type1.data;
            if(double_mark == 2)
            {
                double_mark = 0;
                result_data[(i/SOC_ADC_DIGI_RESULT_BYTES) - 1] = temp_vlts[1];
                result_data[(i/SOC_ADC_DIGI_RESULT_BYTES)] = temp_vlts[0];
            }
        }
    }
}

bool collect_data(uint8_t *data, int data_num, uint16_t* collected_raw_data, uint16_t collected_data_array_size)
{
    uint16_t raw_vlts[3000];
    static uint16_t collected_num = 0;
    bool collected_over_mark = false;
    memset(raw_vlts, 0xcc, sizeof(raw_vlts));
    clean_data(data,data_num,raw_vlts);
    for (uint16_t i = 0; i < data_num/SOC_ADC_DIGI_RESULT_BYTES; i++)
    {
        if(collected_num == collected_data_array_size)
        {
            collected_over_mark = true;
            break;            
        }
        else
        {
            collected_raw_data[collected_num++] = raw_vlts[i];
        }
    }
    if(collected_over_mark)
    {
        
        
        collected_num = 0;
        return true;
    }
    return false;
}