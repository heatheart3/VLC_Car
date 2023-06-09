#include<stdio.h>
#include<string.h>

#define  DECODED_MES_LEN 4

char encode[64];
void transmit_ascii(const char* data)
{
    int tmp =0;
    volatile char bit;
    for(int i=0;i<strlen(data);i++)
    {
        printf("%c : ",data[i]);
        for(int j=7;j>=0;j--)
        {
            bit = (data[i]>>j)&0x01;
            // gpio_set_level(GPIO_OUTPUT_LIGHT,bit);
            // udelay(1000);
            printf("%d",bit);
            if(bit ==1 ) {
                encode[tmp++] = 0;
                encode[tmp++] = 1;
            }
            else
            {
                encode[tmp++] = 1;
                encode[tmp++] = 0;
            }
        }
        printf("\n");
    }

}

void decode(const char *encode, char *data)
{
    // printf("decode_ori:");
    // for (int i = 0; i < VALID_MES_LEN; i++)
    // {
    //     printf("%d", encode[i]);
    // }
    // printf("\n");
    printf("decode_data:");
    for (int i = 0; i < DECODED_MES_LEN; i++)
    {
        int bit_mask = 0;
        for(int j=i*16;j<i*16+16;j+=2) {
            if (encode[j] == 0) {
                if (encode[j + 1] == 0) {
                    printf("Message Error!");
                    break;
                } else {
                    data[i] |= (0x01 << (7 - bit_mask));
                }
            } else {
                if (encode[j + 1] == 1) {
                    printf("Message Error!");
                    break;
                } else {
                    data[i] &= ~(0x01 << (7 - bit_mask));
                }
            }
            bit_mask++;
        }
    }
    printf("%s",data);
    printf("\n");
}

int main()
{
    char data[DECODED_MES_LEN]={0};
    transmit_ascii("scu!");
    decode(encode,data);
    int a = 1;
    for(int i=0;i<64;i++)
        printf("%d",encode[i]);
    return 0;
}