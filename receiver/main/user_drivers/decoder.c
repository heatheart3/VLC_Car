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
    
    decode(raw_vlts,data_num/SOC_ADC_DIGI_RESULT_BYTES, decode_data, decode_data_num);
    
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

uint16_t avg_caculate(uint16_t left, uint16_t right, uint16_t *raw_data, uint16_t data_num)
{
    int sum = 0;
    uint16_t avg = 0;
    //overflow
    if(right >= data_num || left >= data_num)
        return 0xffff;

    for (uint16_t i  = left; i < right; i++)
    {
        sum += raw_data[i];
    }
    avg = sum / (right - left);
    return avg;
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

void raw_data_avg_caculate(uint16_t *raw_data, int data_num, uint16_t *result_data, uint16_t *result_data_num)
{
    uint8_t per_points_num = PERIOD;  // 整个周期的长度
    uint8_t half_per_points_num = per_points_num /2;
    //half_win_left -> half_period_window_left  
    uint16_t half_win_left = 0, half_win_right = 0;
    uint64_t half_per_win_sum = 0;
    uint16_t half_per_win_avg = 0;
    uint16_t avg_count = 0;

    for (int i = 0; i < data_num; i++)
    {
        if(i < half_per_points_num)
        {
            half_per_win_sum += raw_data[i];
        }
        else
        {
            half_per_win_sum += raw_data[half_win_right] - raw_data[half_win_left];
            result_data[avg_count++] = half_per_win_sum/half_per_points_num;
            printf("average:%d\n" ,result_data[avg_count-1]);
        }

        if(i < half_per_points_num)
        {
            half_win_right++;
        }
        else
        {
            half_win_left++;
            half_win_right++;
        }
    }
    *result_data_num = avg_count;
}

void decode(uint16_t *raw_data, int data_num, uint8_t* decode_data, uint16_t* decode_data_num)
{
    uint8_t per_points_num = PERIOD;  // 实际使用的周期长度是164, 这里写法是为了和VOFA波形对应
    uint8_t half_per_points_num = per_points_num /2;
    uint8_t period = per_points_num/2 + 1;
    //half_win_left -> half_period_window_left  
    uint16_t decode_data_cnt = 0;
    bool start_up_mark = false;
    uint16_t goal_value = 0;
    adc_digi_output_data_t *temp_point;
    int8_t trend = 0;
    
    static uint16_t avg_data[5000];
    uint16_t avg_data_num = 0;
    raw_data_avg_caculate(raw_data, data_num, avg_data, &avg_data_num);
    int i = 0;
    while(i < avg_data_num)
    {
        if(start_up_mark)
        {
            uint16_t temp = avg_data[i];
            if(4095 - 2 * avg_data[i] < 0)
                goal_value = 4095;
            else
                goal_value = 0;
            
            if(avg_data[i] != goal_value)
            {
                if(abs(avg_data[i - 1] - goal_value) < abs(avg_data[i + 1] - goal_value))
                    trend = -1;
                else
                    trend = 1;
                // while(temp != goal_value)
                // {
                //     i = i + trend;
                //     //edge condition
                //     // printf("i value:%d\n", i);
                //     if(i >= avg_data_num || i < 0)
                //         break; 
                //     temp = avg_data[i];
                // }
            }
            decode_data[decode_data_cnt++] = decode_half_per(temp);
            // printf("mes:%d\n", decode_data[decode_data_cnt - 1]);
        }
        else
        {
            if(avg_data[i] == 4095)
            {
                decode_data[decode_data_cnt++] = decode_half_per(avg_data[i]);
                start_up_mark = true;
            }
        }
       
        if(start_up_mark)
        {
            i += period;
        }
        else
        {
            i++;
        }
        // printf("i value:%d\n", i);
        if(i > avg_data_num)
            break;
    }

    *decode_data_num = decode_data_cnt;
}
