#include "./include/test.h"

// transmit "0!AaA]&" in a loop
void loop_transmit_ascii()
{
    uint16_t cnt = 0;
    uint8_t mask = 0, temp = 0;
    const uint32_t interval = 100;
    const uint32_t test_period = 7 * interval;
    while (cnt <= test_period + 1)
    {
        if (cnt % interval == 0)
        {
            temp = cnt / interval;
            switch (temp)
            {
            case 1:
                // printf("0\n");
                transmit_ascii("0", GPIO_RIGHT_LIGHT);
                break;
            // case 1:
            case 3:
            case 5:
                //    printf("A\n");
                transmit_ascii("A", GPIO_RIGHT_LIGHT);
                break;
            case 2:
                // printf("!\n");
                transmit_ascii("!", GPIO_RIGHT_LIGHT);
                break;
            case 4:
                // printf("a\n");
                transmit_ascii("a", GPIO_RIGHT_LIGHT);
                break;
            case 6:
                // printf("]\n");
                transmit_ascii("]", GPIO_RIGHT_LIGHT);
                break;
            case 7:
                // printf("&\n");
                transmit_ascii("&", GPIO_RIGHT_LIGHT);
                break;
            }
        }
        else
        {
            gpio_set_level(GPIO_RIGHT_LIGHT, mask);
            mask = 1 - mask;
        }
        cnt++;
        ets_delay_us(TRANSMIT_PERIOD);
    }
}

// test udelay() and  gpio_set_level() time consumption
void test_time_consumption_01()
{
    struct timeval tv_now;
    int64_t time_us_before, time_us_after;
    while (1)
    {
        gettimeofday(&tv_now, NULL);
        time_us_before = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        gpio_set_level(GPIO_RIGHT_LIGHT, 1);
        gettimeofday(&tv_now, NULL);
        time_us_after = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        printf("Set gpio 1=%lld\n", time_us_after - time_us_before);

        gettimeofday(&tv_now, NULL);
        time_us_before = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        ets_delay_us(TRANSMIT_PERIOD);
        gettimeofday(&tv_now, NULL);
        time_us_after = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        printf("udelay=%lld\n", time_us_after - time_us_before);

        gettimeofday(&tv_now, NULL);
        time_us_before = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        gpio_set_level(GPIO_RIGHT_LIGHT, 0);
        gettimeofday(&tv_now, NULL);
        time_us_after = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        printf("Set gpio 0=%lld\n", time_us_after - time_us_before);

        gettimeofday(&tv_now, NULL);
        time_us_before = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        ets_delay_us(TRANSMIT_PERIOD);
        gettimeofday(&tv_now, NULL);
        time_us_after = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        printf("udelay=%lld\n", time_us_after - time_us_before);

        // vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

// test all endoing shceme in one function
void test_allinone()
{
    char *mesA = "abcd";
    char *mesB = "XXUU";
    while (1)
    {
        printf("Transmitting message B: %s\n", mesB);
        uint8_t *symbols = SpinalEncode(mesA);
        uint8_t *test_symbols = SpinalEncode(mesB);
        uint8_t *network_code = network_coding(symbols, test_symbols);

        manchester_OOK(network_code, GPIO_RIGHT_LIGHT);

        for (int i = 0; i < 10000; i++)
        {
            gpio_set_level(GPIO_RIGHT_LIGHT, 1);
            ets_delay_us(TRANSMIT_PERIOD);
            gpio_set_level(GPIO_RIGHT_LIGHT, 0);
            ets_delay_us(TRANSMIT_PERIOD);
        }
    }
}

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

void test0_transmit_nonencode(const char *mes)
{
    while (1)
    {
        gpio_set_level(GPIO_RIGHT_LIGHT, 1);
        ets_delay_us(TRANSMIT_PERIOD);
        gpio_set_level(GPIO_RIGHT_LIGHT, 0);
        ets_delay_us(TRANSMIT_PERIOD);
        gpio_set_level(GPIO_RIGHT_LIGHT, 1);
        ets_delay_us(TRANSMIT_PERIOD);
        gpio_set_level(GPIO_RIGHT_LIGHT, 0);
        ets_delay_us(TRANSMIT_PERIOD);
        transmit_ascii("AAAA", GPIO_RIGHT_LIGHT);

        for (int i = 0; i < 20; i++)
        {
            transmit_ascii(mes, GPIO_RIGHT_LIGHT);
            for (int j = 0; j < 2; j++)
            {
                gpio_set_level(GPIO_RIGHT_LIGHT, 1);
                ets_delay_us(TRANSMIT_PERIOD);
                gpio_set_level(GPIO_RIGHT_LIGHT, 0);
                ets_delay_us(TRANSMIT_PERIOD);
            }
        }
        gpio_set_level(GPIO_RIGHT_LIGHT, 1);
        ets_delay_us(TRANSMIT_PERIOD);
        gpio_set_level(GPIO_RIGHT_LIGHT, 0);
        ets_delay_us(TRANSMIT_PERIOD);
        gpio_set_level(GPIO_RIGHT_LIGHT, 1);
        ets_delay_us(TRANSMIT_PERIOD);
        gpio_set_level(GPIO_RIGHT_LIGHT, 0);
        ets_delay_us(TRANSMIT_PERIOD);
        transmit_ascii("EEEE", GPIO_RIGHT_LIGHT);
    }
}

void test0_transmit_nonencode_ver2(const char *mes)
{
    while (1)
    {
        transmit_ascii(mes, GPIO_RIGHT_LIGHT);
        for (int j = 0; j < 10; j++)
        {
            gpio_set_level(GPIO_RIGHT_LIGHT, 1);
            ets_delay_us(TRANSMIT_PERIOD);
            gpio_set_level(GPIO_RIGHT_LIGHT, 0);
            ets_delay_us(TRANSMIT_PERIOD);
        }
    }
}

void test1_transmit_spinal(const char *mes)
{
    uint8_t *symbols = SpinalEncode(mes);

    uint8_t transmit_tmp[PASS_LENGTH * C] = {0};
    uint8_t tmp_counter = 0;

    for (int i = 0; i < PASS_LENGTH; i++)
    {
        for (int j = C - 1; j >= 0; j--)
        {
            transmit_tmp[tmp_counter++] = (symbols[i] >> j) & 0x01;
        }
    }
    free(symbols);
    while (1)
    {
        // printf("p\n");
        for (int p = 1; p <= PASS; p++)
        {
            // 1. preamble: 011110
            printf("%d\n", p);
            if (p == 1)
            {
                transmit_ook("100001", GPIO_RIGHT_LIGHT);
            }

            transmit_ook(MES_HEADER, GPIO_RIGHT_LIGHT);

            for (int i = (p - 1) * SPINE_LENGTH * C; i < p * SPINE_LENGTH * C; i++)
            {
                if (transmit_tmp[i] == 1)
                {
                    // printf("01");
                    gpio_set_level(GPIO_RIGHT_LIGHT, 0);
                    ets_delay_us(TRANSMIT_PERIOD);
                    gpio_set_level(GPIO_RIGHT_LIGHT, 1);
                    ets_delay_us(TRANSMIT_PERIOD);
                }
                else
                {
                    // printf("10");
                    gpio_set_level(GPIO_RIGHT_LIGHT, 1);
                    ets_delay_us(TRANSMIT_PERIOD);
                    gpio_set_level(GPIO_RIGHT_LIGHT, 0);
                    ets_delay_us(TRANSMIT_PERIOD);
                }
            }
            // printf("\n");
            // 3. postamble: 0
            gpio_set_level(GPIO_RIGHT_LIGHT, 0);
            ets_delay_us(TRANSMIT_PERIOD);

            for (int i = 0; i < 10; i++)
            {
                gpio_set_level(GPIO_RIGHT_LIGHT, 1);
                ets_delay_us(TRANSMIT_PERIOD);
                gpio_set_level(GPIO_RIGHT_LIGHT, 0);
                ets_delay_us(TRANSMIT_PERIOD);
            }
        }

        for (int i = 0; i < 10; i++)
        {
            gpio_set_level(GPIO_RIGHT_LIGHT, 1);
            ets_delay_us(TRANSMIT_PERIOD);
            gpio_set_level(GPIO_RIGHT_LIGHT, 0);
            ets_delay_us(TRANSMIT_PERIOD);
        }
    }
}

void test1_transmit_spinal_ver2(const char *mes)
{
    uint8_t *symbols = SpinalEncode(mes);

    uint8_t transmit_tmp[PASS_LENGTH * C] = {0};
    uint8_t tmp_counter = 0;

    for (int i = 0; i < PASS_LENGTH; i++)
    {
        for (int j = C - 1; j >= 0; j--)
        {
            transmit_tmp[tmp_counter++] = (symbols[i] >> j) & 0x01;
        }
    }
    free(symbols);
    while (1)
    {
        printf("1\n");
        // 1. preamble: 011110
        transmit_ook(MES_HEADER, GPIO_RIGHT_LIGHT);

        for (int i = 0; i < PASS_LENGTH * C; i++)
        {
            if (transmit_tmp[i] == 1)
            {
                // printf("01");
                gpio_set_level(GPIO_RIGHT_LIGHT, 0);
                ets_delay_us(TRANSMIT_PERIOD);
                gpio_set_level(GPIO_RIGHT_LIGHT, 1);
                ets_delay_us(TRANSMIT_PERIOD);
            }
            else
            {
                // printf("10");
                gpio_set_level(GPIO_RIGHT_LIGHT, 1);
                ets_delay_us(TRANSMIT_PERIOD);
                gpio_set_level(GPIO_RIGHT_LIGHT, 0);
                ets_delay_us(TRANSMIT_PERIOD);
            }
        }

        // 3. postamble: 0
        gpio_set_level(GPIO_RIGHT_LIGHT, 0);
        ets_delay_us(TRANSMIT_PERIOD);
        for (int i = 0; i < 20000; i++)
        {
            gpio_set_level(GPIO_RIGHT_LIGHT, 1);
            ets_delay_us(TRANSMIT_PERIOD);
            gpio_set_level(GPIO_RIGHT_LIGHT, 0);
            ets_delay_us(TRANSMIT_PERIOD);
        }
    }
}

void test2_transmit_allinone()
{
    char *mesA = "SCU1";
    char *mesB = "NTU0";

    while (1)
    {
        uint8_t *symbols = SpinalEncode(mesA);
        uint8_t *test_symbols = SpinalEncode(mesB);
        uint8_t *network_code = network_coding(symbols, test_symbols);

        manchester_OOK(network_code, GPIO_RIGHT_LIGHT);

        for (int i = 0; i < 100; i++)
        {
            gpio_set_level(GPIO_RIGHT_LIGHT, 1);
            ets_delay_us(TRANSMIT_PERIOD);
            gpio_set_level(GPIO_RIGHT_LIGHT, 0);
            ets_delay_us(TRANSMIT_PERIOD);
        }
    }
}

void test_get_header()
{

    printf("header\n");
    transmit_ook("01111110", GPIO_RIGHT_LIGHT);
    transmit_ook("0110", GPIO_RIGHT_LIGHT);

    for (int i = 0; i < 20000; i++)
    {
        gpio_set_level(GPIO_RIGHT_LIGHT, 1);
        ets_delay_us(TRANSMIT_PERIOD);
        gpio_set_level(GPIO_RIGHT_LIGHT, 0);
        ets_delay_us(TRANSMIT_PERIOD);
    }
}

void test_transmit_raptor()
{

    // Original Message has 203 char
    uint8_t raptor_k = 203;
    uint8_t raptor_coded_k = 255;

    uint8_t raptor_symbols[255] = {
        97,
        98,
        99,
        100,
        101,
        102,
        103,
        104,
        105,
        106,
        107,
        108,
        109,
        110,
        111,
        112,
        113,
        114,
        115,
        116,
        117,
        118,
        119,
        120,
        121,
        122,
        44,
        97,
        98,
        99,
        100,
        101,
        102,
        103,
        104,
        105,
        106,
        107,
        108,
        109,
        110,
        111,
        112,
        113,
        114,
        115,
        116,
        117,
        118,
        119,
        120,
        121,
        122,
        44,
        97,
        98,
        99,
        100,
        101,
        102,
        103,
        104,
        105,
        106,
        107,
        108,
        109,
        110,
        97,
        98,
        99,
        100,
        101,
        102,
        103,
        104,
        105,
        106,
        107,
        108,
        109,
        110,
        111,
        112,
        113,
        114,
        115,
        116,
        117,
        118,
        119,
        120,
        121,
        122,
        44,
        97,
        98,
        99,
        100,
        101,
        102,
        103,
        104,
        105,
        106,
        107,
        108,
        109,
        110,
        111,
        112,
        113,
        114,
        115,
        116,
        117,
        118,
        119,
        120,
        121,
        122,
        44,
        97,
        98,
        99,
        100,
        101,
        102,
        103,
        104,
        105,
        106,
        107,
        108,
        109,
        110,
        97,
        98,
        99,
        100,
        101,
        102,
        103,
        104,
        105,
        106,
        107,
        108,
        109,
        110,
        111,
        112,
        113,
        114,
        115,
        116,
        117,
        118,
        119,
        120,
        121,
        122,
        44,
        97,
        98,
        99,
        100,
        101,
        102,
        103,
        104,
        105,
        106,
        107,
        108,
        109,
        110,
        111,
        112,
        113,
        114,
        115,
        116,
        117,
        118,
        119,
        120,
        121,
        122,
        44,
        97,
        98,
        99,
        100,
        101,
        102,
        103,
        104,
        105,
        106,
        107,
        108,
        109,
        110,
        65,
        1,
        92,
        106,
        115,
        117,
        78,
        55,
        61,
        59,
        6,
        4,
        3,
        120,
        98,
        83,
        55,
        125,
        53,
        120,
        19,
        0,
        89,
        117,
        103,
        31,
        71,
        65,
        66,
        111,
        30,
        64,
        102,
        52,
        61,
        3,
        106,
        68,
        72,
        120,
        100,
        124,
        75,
        114,
        55,
        2,
        77,
        49,
        29,
        79,
        34};

    // Each frame has 4 chars
    uint8_t packet_per_frame = 4;

    
    // 1.the  MAX SSN, and if the reduant packet is not enough for 4,
    // it'll not be transmitted.
    //2. the max_ssn is up to 255
    uint8_t max_ssn = raptor_coded_k / packet_per_frame;
    while (1)
    {
        for (uint8_t i = 0; i < max_ssn; i++)
        {
            // printf("frame%d\n",i);
            // 1. wait a little bit for the next frame
            for(int j=0;j<100;j++)
            {
                gpio_set_level(GPIO_RIGHT_LIGHT, 1);
                ets_delay_us(TRANSMIT_PERIOD);
                gpio_set_level(GPIO_RIGHT_LIGHT,0);
                ets_delay_us(TRANSMIT_PERIOD);
            }

            // 1. transmit HEADER
            transmit_ook(MES_HEADER, GPIO_RIGHT_LIGHT);

            // 2. transmit SSN
            transmit_8bitz(i, GPIO_RIGHT_LIGHT);

            //3. transmit Symbols
            for (uint8_t j = 0; j < packet_per_frame; j++)
            {
                transmit_8bitz(raptor_symbols[i * packet_per_frame + j], GPIO_RIGHT_LIGHT);
            }

            // 4. transmit the stop bit
            gpio_set_level(GPIO_RIGHT_LIGHT, 0);
            ets_delay_us(TRANSMIT_PERIOD);
        }
    }
}