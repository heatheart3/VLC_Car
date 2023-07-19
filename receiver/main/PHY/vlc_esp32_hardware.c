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

/**
 * @brief Reads a buffer of symbols from a GPIO pin
 * @param buffer: pointer to the buffer where the symbols will be stored
 * @param length: number of symbols to be read
 */
void PHY_read_symbols(uint8_t *buffer, uint16_t length, const int PD_GPIO_NUM)
{
    for (int i = 0; i < length; i++)
    {
        buffer[i] = gpio_get_level(PD_GPIO_NUM);
        ets_delay_us(SAMPLING_DURATION);
    }
}

uint8_t PHY_demoluate_OOK(const uint8_t *buffer, uint16_t *start_index, const uint16_t length, uint8_t *mes_buffer)
{
    uint8_t mes_start_flag = 0;
    uint8_t high_count = 0;
    uint8_t low_count = 0;
    int16_t bit_counter = -1;
    uint8_t edge_dir = 0;
    for (int i = *start_index; i < length; i++)
    {
        if (!mes_start_flag)
        {
            if (buffer[i])
            {
                high_count++;
            }
            else
            {
                if (high_count >= HEADER_THRES)
                {
                    mes_start_flag = true;
                    high_count = 0;
                    bit_counter = -1;
                    edge_dir = 0;
                    ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &s_count));
                }
                else
                {
                    high_count = 0;
                    low_count = 0;
                }
            }
        }

        else
        {
            if (buffer[i])
            {
                high_count++;
            }
            else
            {
                low_count++;
            }

            if (high_count != 0 && low_count != 0)
            {
                // dicard the lsb of header [011"0"]
                if (bit_counter == -1)
                {
                    bit_counter++;
                    if (low_count >= DOUBLE_LOW_THRES)
                    {
                        mes_buffer[bit_counter] = 0;
                        bit_counter++;
                    }
                    low_count = 0;
                    edge_dir = 1;
                }
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
        }

        if (bit_counter >= OOK_SYMBOLS_LEN)
        {
            *start_index = i--;
            // printf("YYY");
            return mes_start_flag;
        }
    }
    // if (mes_start_flag)
    // {
    //     overflow_symbol_counter++;
    //     s_count = 0;;
    //     *start_index=length;
    //     return 0;
    // }
    *start_index = length;
    return mes_start_flag;
}

uint8_t PHY_demoluate_OOK_spinal(const uint8_t *buffer, uint16_t *start_index, const uint16_t length, uint8_t *mes_buffer, uint8_t *header_length)
{
    uint8_t mes_start_flag = 0;
    uint8_t high_count = 0;
    uint8_t low_count = 0;
    int16_t bit_counter = -1;
    uint8_t edge_dir = 0;
    for (int i = *start_index; i < length; i++)
    {
        if (!mes_start_flag)
        {
            if (buffer[i])
            {
                high_count++;
            }
            else
            {
                if (high_count >= HEADER_THRES + 10)
                {
                    mes_start_flag = true;
                    high_count = 0;
                    bit_counter = -1;
                    // ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &s_count));
                    *header_length = 1;
                    edge_dir=0;
                }
                else if (high_count >= HEADER_THRES)
                {
                    mes_start_flag = true;
                    high_count = 0;
                    bit_counter = -1;
                    // ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &s_count));
                    *header_length = 0;
                    edge_dir=0;
                }
                else
                {
                    high_count = 0;
                    low_count = 0;
                }
            }
        }

        else
        {
            if (buffer[i])
            {
                high_count++;
            }
            else
            {
                low_count++;
            }

            if (high_count != 0 && low_count != 0)
            {
                // dicard the lsb of header [011"0"]
                if (bit_counter == -1)
                {
                    bit_counter++;
                    if (low_count >= DOUBLE_LOW_THRES)
                    {
                        mes_buffer[bit_counter] = 0;
                        bit_counter++;
                    }
                    low_count = 0;
                    edge_dir=1;
                }
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
        }

        if (bit_counter >= OOK_SYMBOLS_LEN)
        {
            *start_index = i--;
            // printf("YYY");
            return mes_start_flag;
        }
    }
    // if (mes_start_flag)
    // {
    //     overflow_symbol_counter++;
    //     s_count = 0;
    //     pass_counter = 0;
    //     head = 0;
    //     *start_index = length;
    //     return 0;
    // }
    *start_index = length;
    return mes_start_flag;
}

void PHY_decode_manchester(const uint8_t *symbols, uint8_t *mes)
{
    int j = 0;
    for (int i = 0; i < MANCHESTER_SYMBOLS_LEN; i++)
    {
        if (symbols[j] == 0)
        {
            if (symbols[j + 1] == 0)
            {
                break;
            }
            else
            {
                mes[i] = 1;
            }
        }
        else
        {
            if (symbols[j + 1] == 1)
            {
                break;
            }
            else
            {
                mes[i] = 0;
            }
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
    SpinalDecode(intermediate_symbols, mes);
}

void PHY_decode_allinone(const uint8_t *symbols, uint8_t *mes, uint8_t *symbolsB)
{
    static uint8_t intermediate_symbols[PASS_LENGTH];
    decode_OOK(symbols, intermediate_symbols);
    // for(int i=0;i<PASS_LENGTH;i++)
    // {
    //     printf("%d ",intermediate_symbols[i]);
    // }
    // printf("\n");
    network_decode(intermediate_symbols, symbolsB);
    SpinalDecode(intermediate_symbols, mes);
}
