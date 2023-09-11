#include "./include/test.h"

static uint8_t mes_buffer[OOK_SYMBOLS_LEN];
static uint8_t manchester_symbols[MANCHESTER_SYMBOLS_LEN];
static uint8_t ascii_mes[FRAME_LENGTH];

static uint8_t spinal_mes_buffer[PASS_LENGTH];
static uint8_t spinal_pass_counter = 0;

static uint8_t original_mes[MES_LENGTH];
static uint16_t ssn_counter = 0;

void test_print_PHY_symbols_buffer(uint8_t *buffer, uint32_t length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%d\n", buffer[i]);
    }
    printf("-1\n");
}

void test_get_frame(uint8_t *symbols_buffer, uint32_t symbols_length)
{
    // 1. init the intermediate buffer
    memset(ascii_mes, 0, FRAME_LENGTH * sizeof(uint8_t));
    memset(manchester_symbols, 0, MANCHESTER_SYMBOLS_LEN * sizeof(uint8_t));
    memset(mes_buffer, 0, OOK_SYMBOLS_LEN * sizeof(uint8_t));

    // 2. demodulate
    if (PHY_demodulate_OOK(symbols_buffer, symbols_length, mes_buffer))
    {

        // 3. decode the manchester code, the decoded message will be stored in the manchester_symbols
        PHY_decode_manchester(mes_buffer, manchester_symbols);

        // 4. get CHAR from bits in binary
        vGetASCII(manchester_symbols, ascii_mes);
        ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &e_count));

        // printf("T,%lld\n", e_count - s_count);

        // 5. output the symbols
        printf("%d,", ascii_mes[0]);
        for (int i = 1; i < FRAME_LENGTH; i++)
        {
            printf("%d,", ascii_mes[i]);
        }
        printf("\n");
    }
}

void test_get_original_mes(uint8_t *symbols_buffer, uint32_t symbols_length)
{

    // 1. init the intermediate buffer
    memset(ascii_mes, 0, FRAME_LENGTH * sizeof(uint8_t));
    memset(manchester_symbols, 0, MANCHESTER_SYMBOLS_LEN * sizeof(uint8_t));
    memset(mes_buffer, 0, OOK_SYMBOLS_LEN * sizeof(uint8_t));

    // 2. demodulate
    if (PHY_demodulate_OOK(symbols_buffer, symbols_length, mes_buffer))
    {

        used_frame++;

        // 3. decode the manchester code, the decoded message will be stored in the manchester_symbols
        PHY_decode_manchester(mes_buffer, manchester_symbols);

        // 4. get CHAR from bits in binary
        vGetASCII(manchester_symbols, ascii_mes);

        // printf("T,%lld\n", e_count - s_count);

        // 5. output the symbols

        if (ascii_mes[0] == ssn_counter)
        {
            memcpy(&original_mes[ssn_counter * CHAR_PER_FRAME], &ascii_mes[1], CHAR_PER_FRAME * sizeof(uint8_t));
            ssn_counter++;
        }
        if (ssn_counter >= (MES_LENGTH / CHAR_PER_FRAME))
        {

            if (SHOW_LATENCY)
            {
                // ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &e_count));
                 for (int i = 0; i < (MES_LENGTH); i++)
                {
                    printf("%d", original_mes[i]);
                }
                printf(",");
                printf("%lld\n", used_frame);
                // timer_isr_flag = 0;
                used_frame=0;
            }
            else
            {
                for (int i = 0; i < (MES_LENGTH); i++)
                {
                    printf("%c", original_mes[i]);
                }
                printf("\n");
            }
            ssn_counter = 0;
            memset(original_mes, 0, MES_LENGTH* sizeof(uint8_t));
        }
    }
}