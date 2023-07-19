#include "./include/test.h"

int head=0;
void test_print_PHY_symbols_buffer(uint8_t *buffer, uint32_t length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%d\n", buffer[i]);
    }
}

void test_get_ASCII(const uint8_t *symbols, uint8_t *ch)
{
    for (int i = 0; i < MES_LENGTH; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (symbols[i * 8 + j] == 0)
            {
                ch[i] = ch[i] << 1;
            }
            else
            {
                ch[i] = ch[i] << 1;
                ch[i] = ch[i] | 0x01;
            }
        }
    }
}

void test0_get_packet(uint8_t *symbols_buffer, uint32_t symbols_length)
{
    for (uint16_t i = 0; i < symbols_length; i++)
    {
        if (PHY_demoluate_OOK(symbols_buffer, &i, symbols_length, mes_buffer))
        {
            PHY_decode_manchester(mes_buffer, manchester_symbols);
            test_get_ASCII(manchester_symbols, ascii_mes);

            if (ascii_mes[0] == 'A')
            {
                overflow_symbol_counter = 0;
                decode_right_conuter = 0;
            }
            else if (ascii_mes[0] == 'E')
            {
                // printf("%ld,%ld\n",decode_right_conuter,overflow_symbol_counter);
                output_buffer[pass_counter] = decode_right_conuter;
                pass_counter++;
                decode_right_conuter = 0;
                overflow_symbol_counter = 0;
            }

            if (pass_counter >= 10)
            {
                for (int i = 0; i < 10; i++)
                {
                    printf("%d\n", output_buffer[i]);
                }
                printf("------END-------\n");
                pass_counter = 0;
            }
            memset(ascii_mes, 0, MES_LENGTH * sizeof(uint8_t));
            memset(mes_buffer, 0, OOK_SYMBOLS_LEN * sizeof(uint8_t));
        }
    }
}

uint32_t test0_get_packet_ver2(uint8_t *symbols_buffer, uint32_t symbols_length)
{
    uint32_t ret=0;
    for (uint16_t i = 0; i < symbols_length; i++)
    {
        if (PHY_demoluate_OOK(symbols_buffer, &i, symbols_length, mes_buffer))
        {
            PHY_decode_manchester(mes_buffer, manchester_symbols);
            test_get_ASCII(manchester_symbols, ascii_mes);
            if(ascii_mes[0]==70&&ascii_mes[1]==111&&ascii_mes[2]==49&&ascii_mes[3]==33)
            {
                ret++;
            }

            // for (int j = 0; j < MES_LENGTH; j++)
            // {
            //     printf("%d,", ascii_mes[j]);
            // }
            // printf("\n");

            memset(ascii_mes, 0, MES_LENGTH * sizeof(uint8_t));
            memset(mes_buffer, 0, OOK_SYMBOLS_LEN * sizeof(uint8_t));
        }
    }
    return ret;
}


void test1_get_packet_spinal(uint8_t *symbols_buffer, uint32_t symbols_length)
{

    for (uint16_t i = 0; i < symbols_length; i++)
    {
        if (PHY_demoluate_OOK_spinal(symbols_buffer, &i, symbols_length, mes_buffer,&head))
        {
            // for(int i=0;i<OOK_SYMBOLS_LEN;i++)
            // {
            //     printf("%d",mes_buffer[i]);
            // }
            // printf("\n");
            if (pass_counter < PASS)
            {
                int index = 1;
                uint8_t symbols[SPINE_LENGTH];
                printf("p%d:", pass_counter);
                for (int X = 0; X< SPINE_LENGTH; X++)
                {
                    symbols[X] = 0;
                    for (int j = 0; j < C; j++)
                    {
                        symbols[X] |= mes_buffer[index];
                        index += 2;
                        symbols[X] <<= 1;
                    }
                    symbols[X] >>= 1;
                    printf("%d,", symbols[X]);
                }
                printf("\n");
                if(head==1)
                {
                    memcpy(spinal_mes_buffer, mes_buffer, OOK_SYMBOLS_LEN * sizeof(uint8_t));
                    pass_counter=1;
                }
                else if(pass_counter>=1)
                {
                    memcpy(&spinal_mes_buffer[pass_counter*OOK_SYMBOLS_LEN], mes_buffer, OOK_SYMBOLS_LEN * sizeof(uint8_t));
                    pass_counter++;
                }
            }
            if (pass_counter == PASS)
            {
                head=0;
                pass_counter = 0;
                PHY_decode_spinal(spinal_mes_buffer, ascii_mes);
                for (int j = 0; j < MES_LENGTH; j++)
                {
                    printf("%d,", ascii_mes[j]);
                }
                printf("\n");
    
                memset(spinal_mes_buffer,0,OOK_SYMBOLS_LEN*PASS*sizeof(uint8_t));
            }
            memset(ascii_mes, 0, MES_LENGTH * sizeof(uint8_t));
            memset(mes_buffer, 0, OOK_SYMBOLS_LEN * sizeof(uint8_t));
        }
    }
}


void test1_get_packet_spinal_ver2(uint8_t *symbols_buffer, uint32_t symbols_length)
{

    for (uint16_t i = 0; i < symbols_length; i++)
    {
        if (PHY_demoluate_OOK(symbols_buffer, &i, symbols_length, mes_buffer))
        {
            // for(int i=0;i<OOK_SYMBOLS_LEN;i++)
            // {
            //     printf("%d",mes_buffer[i]);
            // }
            // printf("\n");
            PHY_decode_spinal(mes_buffer, ascii_mes);
            for (int j = 0; j < MES_LENGTH; j++)
            {
                printf("%d,", ascii_mes[j]);
            }
            printf("\n");
            memset(ascii_mes, 0, MES_LENGTH * sizeof(uint8_t));
            memset(mes_buffer, 0, OOK_SYMBOLS_LEN * sizeof(uint8_t));
        }
    }
}

void test2_get_overall_latency(uint8_t *symbols_buffer, uint32_t symbols_length, uint8_t *symbolsB)
{
    for (uint16_t i = 0; i < symbols_length; i++)
    {
        if (PHY_demoluate_OOK(symbols_buffer, &i, symbols_length, mes_buffer))
        {
            // for(int j=0;j<OOK_SYMBOLS_LEN;j++)
            // {
            //     printf("%d",mes_buffer[j]);
            // }
            // printf("\n");
            PHY_decode_allinone(mes_buffer, ascii_mes, symbolsB);
            ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &e_count));
            if (ascii_mes[0] == 'S' && ascii_mes[1] == 'C' && ascii_mes[2] == 'U' && ascii_mes[3] == '1')
            {
                if (s_error_count != 0)
                    printf("N,%lld\n", e_count - s_error_count);
                else
                    printf("Y,%lld\n", e_count - s_count);

                s_error_count = 0;
            }
            else if (s_error_count == 0)
            {
                s_error_count = s_count;
            }
            memset(ascii_mes, 0, MES_LENGTH);
            memset(mes_buffer, 0, OOK_SYMBOLS_LEN);
        }
    }
}