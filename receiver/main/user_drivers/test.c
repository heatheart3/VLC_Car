#include "./include/test.h"


void test_print_PHY_symbols_buffer(uint8_t *buffer, uint32_t length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%d\n", buffer[i]);
    }
    // printf("-4000\n");
}

void test_get_ASCII(const uint8_t *symbols, uint8_t *ch)
{
    for (int i = 0; i < ASCII_MES_LEN; i++)
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

            if(ascii_mes[0]=='A')
            {
                overflow_symbol_counter=0;
                decode_right_conuter=0;
            }
            else if(ascii_mes[0]=='E')
            {
                // printf("%ld,%ld\n",decode_right_conuter,overflow_symbol_counter);
                output_buffer[pass_counter]=decode_right_conuter;
                pass_counter++;
                decode_right_conuter=0;
                overflow_symbol_counter=0;
            }

            if(pass_counter>=10)
            {
                for(int i=0;i<10;i++)
                {
                    printf("%d\n",output_buffer[i]);
                }
                printf("------END-------\n");
                pass_counter=0;
            }
            memset(ascii_mes, 0, ASCII_MES_LEN);
            memset(mes_buffer, 0, OOK_SYMBOLS_LEN);
        }
    }
}void test0_get_packet_ver2(uint8_t *symbols_buffer, uint32_t symbols_length)
{
    for (uint16_t i = 0; i < symbols_length; i++)
    {
        if (PHY_demoluate_OOK(symbols_buffer, &i, symbols_length, mes_buffer))
        {
            PHY_decode_manchester(mes_buffer, manchester_symbols);
            test_get_ASCII(manchester_symbols, ascii_mes);

            for (int j = 0; j < ASCII_MES_LEN; j++)
            {
                printf("%d,", ascii_mes[j]);
            }
            printf("\n");

            memset(ascii_mes, 0, ASCII_MES_LEN);
            memset(mes_buffer, 0, OOK_SYMBOLS_LEN);
        }
    }
}



void test1_get_packet_spinal(uint8_t *symbols_buffer, uint32_t symbols_length)
{
    for (uint16_t i = 0; i < symbols_length; i++)
    {
        if (PHY_demoluate_OOK(symbols_buffer, &i, symbols_length, mes_buffer))
        {
            PHY_decode_spinal(mes_buffer, ascii_mes);
            for (int j = 0; j < ASCII_MES_LEN; j++)
            {
                printf("%d,", ascii_mes[j]);
            }
            printf("\n");
            memset(ascii_mes, 0, ASCII_MES_LEN);
            memset(mes_buffer, 0, OOK_SYMBOLS_LEN);
        }
    }
}