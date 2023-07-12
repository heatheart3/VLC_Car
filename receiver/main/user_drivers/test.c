#include "./include/test.h"

void test_print_PHY_symbols_buffer(uint8_t *buffer, uint32_t length)
{
    for(int i = 0; i < length; i++)
    {
        printf("%d\n", buffer[i]);
    }
    printf("-4000\n");
}


void test_get_ASCII(const uint8_t* symbols, uint8_t* ch)
{
    for(int i=0;i<ASCII_MES_LEN;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(symbols[i*8+j]==0)
            {
                ch[i]=ch[i]<<1;
            }
            else
            {
                ch[i]=ch[i]<<1;
                ch[i]=ch[i]|0x01;
            }
        }
    }   
}