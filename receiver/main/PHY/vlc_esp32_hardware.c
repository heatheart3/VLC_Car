#include "./include/vlc_esp32_hardware.h"





/**
 * @brief Configures a GPIO pin as input
 */
void PHY_gpio_config(const int gpio_num)
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << gpio_num);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
}

void PHY_read_symbols(uint8_t *buffer, uint16_t length, const int PD_GPIO_NUM)
{
    uint8_t header_counter=0;
    while(1)
    {
        if(gpio_get_level(PD_GPIO_NUM))
        {
            header_counter++;
        }
        else
        {
        if(header_counter>=HEADER_THRES)
        {
            // if(!timer_isr_flag)
            // {
                ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &s_count));
            //     timer_isr_flag=1;
            // }
            header_counter=0;
            break;
        }
            header_counter=0;
        }
        ets_delay_us(SAMPLING_DURATION);
    }

    
    for (int i = 0; i < length; i++)
    {
        buffer[i] = gpio_get_level(PD_GPIO_NUM);
        ets_delay_us(SAMPLING_DURATION);
    }
}


void PHY_easy_read_symbols(uint8_t *buffer, uint16_t length, const int PD_GPIO_NUM)
{
    for (int i = 0; i < length; i++)
    {
        buffer[i] = gpio_get_level(PD_GPIO_NUM);
        ets_delay_us(SAMPLING_DURATION);
    }
}


uint8_t PHY_demodulate_OOK(const uint8_t *buffer, const uint16_t length, uint8_t *mes_buffer)
{
        uint8_t high_count = 0;
        uint8_t low_count = 0;
        int16_t bit_counter = -1;
        uint8_t edge_dir = 0;
        for (int i = 0 ; i < length; i++)
    {

            // 0. search the buffer to get the duration of the signal
            if (buffer[i])
            {
                high_count++;
            }
            else
            {
                low_count++;
            }
            

            
            // 1. if detect edge, means a message is coming
            // 2. start to demodulate the message bits from the signal duration
            if (high_count != 0 && low_count != 0)
            {

                // 1. get the first bit
                if (bit_counter == -1)
                {

                    //eliminate the header noise
                    if(low_count <=5)
                    {
                        high_count=0;
                        continue;
                    }
                    bit_counter++;
                    if (low_count >= HEADER_LOW_THRES)
                    {
                        mes_buffer[bit_counter] = 0;
                        bit_counter++;
                    }
                    low_count = 0;
                    edge_dir = 1;
                }
                
                
                // 2. get the following bit according to the edge direction and signal duration

                // TODO: eliminate the noise in the signal
                else if (bit_counter < OOK_SYMBOLS_LEN)
                {
                    if (edge_dir == 1)
                    {
                        if (high_count >= DOUBLE_HIGH_THRES)
                        {
                            mes_buffer[bit_counter] = 1;
                            bit_counter++;
                            mes_buffer[bit_counter] = 1;
                            bit_counter++;
                            high_count = 0;
                        }
                        else
                        {
                            mes_buffer[bit_counter] = 1;
                            bit_counter++;
                            high_count = 0;
                        }
                        edge_dir = 0;
                    }
                    else
                    {
                        if (low_count >= DOUBLE_LOW_THRES)
                        {
                            mes_buffer[bit_counter] = 0;
                            bit_counter++;
                            mes_buffer[bit_counter] = 0;
                            bit_counter++;
                            low_count = 0;
                        }
                        else
                        {
                            mes_buffer[bit_counter] = 0;
                            bit_counter++;
                            low_count = 0;
                        }
                        edge_dir = 1;
                    }
                }
            }
        
        // 3. if collect enough bits from this frame
        //    then break
        if (bit_counter >= OOK_SYMBOLS_LEN)
        {
            return 1;
        }
    }

    

    // 4. else return 0, but mainly this will not happen
    return 0;
}



void PHY_decode_manchester(const uint8_t *symbols, uint8_t *mes)
{
    int j = 0;
    for (int i = 0; i < MANCHESTER_SYMBOLS_LEN; i++)
    {
        if (symbols[j] == 0)
        {  
                mes[i] = 1;
        }
        else
        {
                mes[i] = 0;
        }
        j += 2;
    }
}

void inline spinal_get_intermediate_symbols(const uint8_t *symbols, uint8_t *intermediate_symbols)
{
    int index = 1;
    for (int i = 0; i < PASS_LENGTH; i++)
    {
        intermediate_symbols[i] = 0;
        for (int j = 0; j < C; j++)
        {
            intermediate_symbols[i] |= symbols[index];
            index++;
            intermediate_symbols[i] <<= 1;
        }
        intermediate_symbols[i] >>= 1;
    }
}

void PHY_decode_spinal(const uint8_t *symbols, uint8_t *mes)
{

    static uint8_t intermediate_symbols[PASS_LENGTH];
    // for(int i=0;i<PASS_LENGTH*C*2;i++)
    // {
    //     printf("%d",symbols[i]);
    // }
    // printf("\n");
    decode_OOK(symbols, intermediate_symbols);
    // for(int i=0;i<PASS_LENGTH;i++)
    // {
    //     printf("%d ",intermediate_symbols[i]);
    // }
    // printf("\n");
    SpinalDecode(intermediate_symbols, mes);
}

