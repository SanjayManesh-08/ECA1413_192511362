#include <LPC214X.H>
#define DS3      (1U << 13)
#define DS4      (1U << 12)
#define SEG_CODE (0xFFU << 16)

unsigned char const seg_dat[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x67
};
void delayms(int n)
{
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < 5035; j++) {
            ;
        }
    }
}

int main(void)
{
    unsigned char count;
    PINSEL0 = 0;  
    PINSEL1 = 0;  

    IODIR0 = SEG_CODE | DS3 | DS4;

    IOSET0 = SEG_CODE | DS3;  
    IOCLR0 = DS4;             
    count = 0;

    IOCLR0 = SEG_CODE;
    IOSET0 = ((unsigned int)seg_dat[count] << 16);

    while (1) {
        delayms(1000);

        count++;

        if (count > 9) {
            count = 0;
        }
        IOCLR0 = SEG_CODE;
        IOSET0 = ((unsigned int)seg_dat[count] << 16);
    }
}
