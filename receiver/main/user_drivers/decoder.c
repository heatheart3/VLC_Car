#include "./include/decoder.h"

uint8_t decode_half_per(uint16_t avg)
{
    if(avg > 3400)
    {   
        return 1;
    }
    return 0;
}

void decode(uint8_t *data, int data_num, uint8_t* decode_data,  uint16_t* decode_data_num)
{

    uint16_t temp_vlt = 0;    
    uint8_t per_points_num = 164;
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
    uint8_t double_mark = 0;
    uint16_t real_vlts[3000];

    // vTaskDelay(1000 / portTICK_PERIOD_MS);
    for (int i = 0; i < data_num; i += SOC_ADC_DIGI_RESULT_BYTES)
    {
        adc_digi_output_data_t *p = (void *)&data[i];
        if (check_valid_data(p))
        {
            temp_vlt = p->type1.data;

            //Half Window update
            if(i < half_per_points_num * SOC_ADC_DIGI_RESULT_BYTES)
            {
                // Half Window is starting up
                half_per_win_sum += p->type1.data;
                half_win_right += SOC_ADC_DIGI_RESULT_BYTES;
            }        
            else
            {
                if(i == half_per_points_num * SOC_ADC_DIGI_RESULT_BYTES)
                {
                    start_up_mark = true;
                }
                //Half Window size has expand to half period points num
                temp_point = (void *) &data[half_win_left];
                half_per_win_sum += p->type1.data - temp_point->type1.data;
                half_per_win_avg = half_per_win_sum / half_per_points_num;
                half_win_left += SOC_ADC_DIGI_RESULT_BYTES;
                half_win_right += SOC_ADC_DIGI_RESULT_BYTES;
            }
            // printf("sum:%lld\n", half_per_win_sum);
            // printf("avg:%d\n", half_per_win_avg);
            // printf("value:%d\n", temp_vlt);

            temp_vlts[double_mark++] = temp_vlt;
            if(double_mark == 2)
            {
                double_mark = 0;
                printf("value:%ld\nvalue:%ld\n", temp_vlts[1], temp_vlts[0]);
            }

            if(start_up_mark)
            {
                if(steady_mark)
                {
                    if((i - steady_edge) % (half_per_points_num) == 0)
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
                    if(half_per_win_avg >= 4000)
                    {
                        steady_mark = true;
                        decode_data[decode_data_cnt++] = decode_half_per(half_per_win_avg);
                        steady_edge = half_win_left;
                        // printf("edge:%d\n", steady_edge);
                        // printf("value:-2000\n");
                    }
                }
            }

        }
    }
    *decode_data_num = decode_data_cnt;
}



