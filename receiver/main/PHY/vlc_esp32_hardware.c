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

void PHY_demoluate_OOK(const uint8_t *buffer, uint16_t *start_index, const uint16_t length, uint8_t *mes_buffer)
{
    uint8_t mes_start_flag = 0;
    uint8_t high_count = 0;
    uint8_t low_count = 0;
    int8_t bit_counter = 0;
    for(int i=*start_index; i<length; i++)
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
                }
                else if (bit_counter < OOK_SYMBOLS_LEN)
                {
                    if (high_count > low_count)
                    {
                        if (high_count >= DOUBLE_HIGH_THRES)
                        {
                            mes_buffer[bit_counter] = 1;
                            bit_counter++;
                            mes_buffer[bit_counter] = 1;
                            bit_counter++;
                            high_count = 0;
                        }
                        else if (high_count >= SINGLE_HIGH_THRES && high_count < DOUBLE_HIGH_THRES)
                        {
                            mes_buffer[bit_counter] = 1;
                            bit_counter++;
                            high_count = 0;
                        }
                        else
                        {
                            printf("ERROR: Invalid signal duration!\n");
                            return;
                        }
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
                        else if (low_count >= SINGLE_LOW_THRES && low_count < DOUBLE_LOW_THRES)
                        {
                            mes_buffer[bit_counter] = 0;
                            bit_counter++;
                            low_count = 0;
                        }
                        else
                        {
                            printf("ERROR: Invalid signal duration!\n");
                            return;
                        }
                    }
                }
            }
        }
    
        if (bit_counter == OOK_SYMBOLS_LEN)
        {
            *start_index = i--;
            return;
        }
    }
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
