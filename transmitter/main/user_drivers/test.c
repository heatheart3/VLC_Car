#include "./include/test.h"

void test_square_wave()
{
    gpio_set_level(GPIO_RIGHT_LIGHT, 1);
    ets_delay_us(TRANSMIT_PERIOD);
    gpio_set_level(GPIO_RIGHT_LIGHT, 0);
    ets_delay_us(TRANSMIT_PERIOD);
}

void test_10_1100()
{
    while (1)
    {
        gpio_set_level(GPIO_RIGHT_LIGHT, 1);
        ets_delay_us(TRANSMIT_PERIOD);
        gpio_set_level(GPIO_RIGHT_LIGHT, 0);
        ets_delay_us(TRANSMIT_PERIOD);

        gpio_set_level(GPIO_RIGHT_LIGHT, 1);
        ets_delay_us(TRANSMIT_PERIOD);
        gpio_set_level(GPIO_RIGHT_LIGHT, 1);
        ets_delay_us(TRANSMIT_PERIOD);
        gpio_set_level(GPIO_RIGHT_LIGHT, 0);
        ets_delay_us(TRANSMIT_PERIOD);
        gpio_set_level(GPIO_RIGHT_LIGHT, 0);
        ets_delay_us(TRANSMIT_PERIOD);
    }
}



void test_transmit_original(const char* mes)
{
    // 1. 8 bits per char
    uint16_t mes_length = strlen(mes);

    // 1.the  MAX SSN, and if the reduant packet is not enough for 4,
    // it'll not be transmitted.
    //2. the max_ssn is up to 255
    uint8_t max_ssn = mes_length/CHAR_PER_FRAME;
    uint8_t need_add_up_packets = mes_length%CHAR_PER_FRAME; // no more packets, so need add zero.
    if(need_add_up_packets!=0)
    {
        max_ssn++;
    }
    // printf("max_ssn:%d\n",max_ssn);
    while (1)
    {
        for (uint8_t i = 0; i < max_ssn; i++)
        {
            // printf("frame%d\n",i);
            // 1. wait a little bit for the next frame
            for(int j=0;j<60;j++)
            {
                gpio_set_level(GPIO_RIGHT_LIGHT, 1);
                ets_delay_us(TRANSMIT_PERIOD);
                gpio_set_level(GPIO_RIGHT_LIGHT,0);
                ets_delay_us(TRANSMIT_PERIOD);
            }

            // 1. transmit HEADER
            transmit_ook(FRAME_HEADER, GPIO_RIGHT_LIGHT);

            // 2. transmit SSN
            transmit_8bitz(i, GPIO_RIGHT_LIGHT);

            //3. transmit Symbols
            for (uint8_t j = 0; j < CHAR_PER_FRAME; j++)
            {
                // if the packet is not enough for 4, "0" data is sent
                if((i*CHAR_PER_FRAME+j)>=mes_length)
                {
                    transmit_8bitz(0, GPIO_RIGHT_LIGHT);
                    // printf("0\n");
                }
                else
                {
                    transmit_8bitz(mes[i * CHAR_PER_FRAME + j], GPIO_RIGHT_LIGHT);
                    // printf("%c\n",mes[i * CHAR_PER_FRAME + j]);
                }
            }

            // 4. transmit the stop bit
            gpio_set_level(GPIO_RIGHT_LIGHT, 0);
            ets_delay_us(TRANSMIT_PERIOD);
        }
    }
}


void test_transmit_raptor()
{

    // uint8_t raptor_k = 50;
    // uint8_t raptor_coded_k = 62;
    // uint8_t raptor_symbols[62]={
    //   97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 49, 50, 51, 44, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 86, 74, 72, 54, 6, 20, 98, 114, 69, 7, 48, 8
    // };

    uint8_t raptor_k = 100;
    uint8_t raptor_coded_k = 125;
    uint8_t raptor_symbols[125]={
    97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 49, 50, 51, 44, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 49, 50, 51, 44, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 49, 50, 51, 44, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 81, 113, 32, 69, 45, 81, 25, 51, 5, 50, 28, 108, 117, 98, 33, 66, 50, 62, 61, 22, 104, 64, 52, 101, 21
    };

    // uint8_t raptor_k = 204;
    // uint8_t raptor_coded_k = 255;
    // uint8_t raptor_symbols[255] = {
    //     97,
    //     98,
    //     99,
    //     100,
    //     101,
    //     102,
    //     103,
    //     104,
    //     105,
    //     106,
    //     107,
    //     108,
    //     109,
    //     110,
    //     111,
    //     112,
    //     113,
    //     114,
    //     115,
    //     116,
    //     117,
    //     118,
    //     119,
    //     120,
    //     121,
    //     122,
    //     44,
    //     97,
    //     98,
    //     99,
    //     100,
    //     101,
    //     102,
    //     103,
    //     104,
    //     105,
    //     106,
    //     107,
    //     108,
    //     109,
    //     110,
    //     111,
    //     112,
    //     113,
    //     114,
    //     115,
    //     116,
    //     117,
    //     118,
    //     119,
    //     120,
    //     121,
    //     122,
    //     44,
    //     97,
    //     98,
    //     99,
    //     100,
    //     101,
    //     102,
    //     103,
    //     104,
    //     105,
    //     106,
    //     107,
    //     108,
    //     109,
    //     110,
    //     97,
    //     98,
    //     99,
    //     100,
    //     101,
    //     102,
    //     103,
    //     104,
    //     105,
    //     106,
    //     107,
    //     108,
    //     109,
    //     110,
    //     111,
    //     112,
    //     113,
    //     114,
    //     115,
    //     116,
    //     117,
    //     118,
    //     119,
    //     120,
    //     121,
    //     122,
    //     44,
    //     97,
    //     98,
    //     99,
    //     100,
    //     101,
    //     102,
    //     103,
    //     104,
    //     105,
    //     106,
    //     107,
    //     108,
    //     109,
    //     110,
    //     111,
    //     112,
    //     113,
    //     114,
    //     115,
    //     116,
    //     117,
    //     118,
    //     119,
    //     120,
    //     121,
    //     122,
    //     44,
    //     97,
    //     98,
    //     99,
    //     100,
    //     101,
    //     102,
    //     103,
    //     104,
    //     105,
    //     106,
    //     107,
    //     108,
    //     109,
    //     110,
    //     97,
    //     98,
    //     99,
    //     100,
    //     101,
    //     102,
    //     103,
    //     104,
    //     105,
    //     106,
    //     107,
    //     108,
    //     109,
    //     110,
    //     111,
    //     112,
    //     113,
    //     114,
    //     115,
    //     116,
    //     117,
    //     118,
    //     119,
    //     120,
    //     121,
    //     122,
    //     44,
    //     97,
    //     98,
    //     99,
    //     100,
    //     101,
    //     102,
    //     103,
    //     104,
    //     105,
    //     106,
    //     107,
    //     108,
    //     109,
    //     110,
    //     111,
    //     112,
    //     113,
    //     114,
    //     115,
    //     116,
    //     117,
    //     118,
    //     119,
    //     120,
    //     121,
    //     122,
    //     44,
    //     97,
    //     98,
    //     99,
    //     100,
    //     101,
    //     102,
    //     103,
    //     104,
    //     105,
    //     106,
    //     107,
    //     108,
    //     109,
    //     110,
    //     65,
    //     1,
    //     92,
    //     106,
    //     115,
    //     117,
    //     78,
    //     55,
    //     61,
    //     59,
    //     6,
    //     4,
    //     3,
    //     120,
    //     98,
    //     83,
    //     55,
    //     125,
    //     53,
    //     120,
    //     19,
    //     0,
    //     89,
    //     117,
    //     103,
    //     31,
    //     71,
    //     65,
    //     66,
    //     111,
    //     30,
    //     64,
    //     102,
    //     52,
    //     61,
    //     3,
    //     106,
    //     68,
    //     72,
    //     120,
    //     100,
    //     124,
    //     75,
    //     114,
    //     55,
    //     2,
    //     77,
    //     49,
    //     29,
    //     79,
    //     34};
    
    // Each frame has 4 chars
    uint8_t packet_per_frame = 4;

    
    // 1.the  MAX SSN, and if the reduant packet is not enough for 4,
    // it'll not be transmitted.
    //2. the max_ssn is up to 255

    uint8_t max_ssn = raptor_coded_k / CHAR_PER_FRAME;
    uint8_t need_add_up_packets = raptor_coded_k%CHAR_PER_FRAME; // no more packets, so need add zero.
    if(need_add_up_packets!=0)
    {
        max_ssn++;
    }
    while (1)
    {
        for (uint8_t i = 0; i < max_ssn; i++)
        {
            // printf("frame%d\n",i);
            // 1. wait a little bit for the next frame
            for(int j=0;j<60;j++)
            {
                gpio_set_level(GPIO_RIGHT_LIGHT, 1);
                ets_delay_us(TRANSMIT_PERIOD);
                gpio_set_level(GPIO_RIGHT_LIGHT,0);
                ets_delay_us(TRANSMIT_PERIOD);
            }

            // 1. transmit HEADER
            transmit_ook(FRAME_HEADER, GPIO_RIGHT_LIGHT);

            // 2. transmit SSN
            transmit_8bitz(i, GPIO_RIGHT_LIGHT);

            //3. transmit Symbols
            for (uint8_t j = 0; j < packet_per_frame; j++)
            {
                //   if the packet is not enough for 4, "0" data is sent
                if((i*CHAR_PER_FRAME+j)>=raptor_coded_k)
                {
                    transmit_8bitz(0, GPIO_RIGHT_LIGHT);
                    // printf("0\n");
                }
                else
                {
                    transmit_8bitz(raptor_symbols[i * CHAR_PER_FRAME + j], GPIO_RIGHT_LIGHT);
                    // printf("%c\n",mes[i * CHAR_PER_FRAME + j]);
                }
            }

            // 4. transmit the stop bit
            gpio_set_level(GPIO_RIGHT_LIGHT, 0);
            ets_delay_us(TRANSMIT_PERIOD);
        }
    }
}