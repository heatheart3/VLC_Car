#include "./include/test.h"

void test_print_PHY_symbols_buffer(uint8_t *buffer, uint32_t length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%d\n", buffer[i]);
    }
    printf("-4000\n");
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
            for (int j = 0; j < ASCII_MES_LEN; j++)
            {
                printf("%c", ascii_mes[j]);
            }
            printf("\n");
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
                printf("%c", ascii_mes[j]);
            }
            printf("\n");
        }
    }
}