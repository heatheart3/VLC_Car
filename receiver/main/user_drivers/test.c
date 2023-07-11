#include "./include/test.h"

void print_PHY_symbols_buffer(uint8_t *buffer, uint32_t length)
{
    for(int i = 0; i < length; i++)
    {
        printf("%d\n", buffer[i]);
    }
    printf("-4000\n");
}