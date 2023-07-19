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
        for (int j = 0; j < 20; j++)
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
        for (int p = 1; p <= PASS; p++)
        {
            // printf("p%d:", p);
            // 1. preamble: 011110

            if (p == 1)
                transmit_ook("01111110", GPIO_RIGHT_LIGHT);
            else
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

            for (int i = 0; i < 5; i++)
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
        for (int i = 0; i < 20; i++)
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