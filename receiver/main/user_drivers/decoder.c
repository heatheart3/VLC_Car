#include "./include/decoder.h"

extern int overflow_mark;

uint8_t decode_half_per(uint16_t avg)
{
    if(avg > 3900)
    {   
        return 1;
    }
    return 0;
}

void decoder(uint8_t *data, int data_num, uint8_t* decode_data,  uint16_t* decode_data_num)
{
    uint16_t raw_vlts[3000];
    memset(raw_vlts, 0xcc, sizeof(raw_vlts));
    clean_data(data,data_num,raw_vlts);
    
    decode(raw_vlts,data_num, decode_data, decode_data_num);
    
}


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

void decode(uint16_t *raw_data, int data_num, uint8_t* decode_data, uint16_t* decode_data_num)
{
    uint16_t temp_vlt = 0;    
    uint8_t per_points_num = 162;  // 实际使用的周期长度是164, 这里写法是为了和VOFA波形对应
    uint8_t half_per_points_num = per_points_num /2;
    //half_win_left -> half_period_window_left  
    uint16_t half_win_left = 0, half_win_right = 0;
    uint16_t whole_per_left, whole_per_right;
    uint64_t half_per_win_sum = 0, whole_per_win_sum = 0;
    uint16_t half_per_win_avg = 0, whole_per_win_avg;
    uint16_t win_start_up_cnt = 0, decode_data_cnt = 0;
    bool steady_mark  = false, start_up_mark = false;
    uint16_t steady_edge = 0;
    adc_digi_output_data_t *temp_point;

    
    for(int i = 0; i < data_num/SOC_ADC_DIGI_RESULT_BYTES; i++)
    {
        if(i < half_per_points_num)
            {
                // Half Window is starting up
                half_per_win_sum += raw_data[i];
                half_win_right ++;
            }        
            else
            {
                if(i == half_per_points_num)
                {
                    start_up_mark = true;
                }
                //Half Window size has expand to half period points num
                half_per_win_sum += raw_data[i] - raw_data[half_win_left];
                half_per_win_avg = half_per_win_sum / half_per_points_num;
                half_win_left ++;
                half_win_right ++;
            }
            // printf("sum:%lld\n", half_per_win_sum);
            // printf("avg:%d\n", half_per_win_avg);
            printf("value:%d\n", raw_data[i]);
             if(overflow_mark == 1)
            {
                ESP_LOGI("POOL", "OVERFLOW");
                printf("value:-8000\n");
                overflow_mark = 0;
            }

            if(start_up_mark)
            {
                if(steady_mark)
                {
                    if((i - steady_edge) % (half_per_points_num + 1) == 0) // 半个周期实际是81 + 1
                    {
                        // uint16_t temp_delta = i - steady_edge;
                        // printf("i:%d\n", temp_delta);
                        // printf("avg:%d\n", half_per_win_avg);
                        decode_data[decode_data_cnt++] = decode_half_per(half_per_win_avg);
                    }
                }
                else
                {
                    //whole trend is a horizenal line
                    if(half_per_win_avg == 4095)
                    {
                        steady_mark = true;
                        decode_data[decode_data_cnt++] = decode_half_per(half_per_win_avg);
                        steady_edge = i;
                        // printf("edge:%d\n", steady_edge);
                        // printf("value:-2000\n");
                    }
                }
            }
    }
    
    *decode_data_num = decode_data_cnt;
}
