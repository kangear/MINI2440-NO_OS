/*
 * init.c: 进行一些初始化
 */ 

#include "s3c24xx.h"

/*
 * LED1-4对应GPB5、GPB6、GPB7、GPB8
 */
#define GPB5_out        (1<<(5*2))      // LED1
#define GPB6_out        (1<<(6*2))      // LED2
#define GPB7_out        (1<<(7*2))      // LED3
#define GPB8_out        (1<<(8*2))      // LED4


void init_led(void)
{
    GPBCON = GPB5_out | GPB6_out | GPB7_out | GPB8_out ;
}


void clean_bss(void)
{
    extern int __bss_start, __bss_end;
    int *p = &__bss_start;

    for (; p < &__bss_end; p++)
        *p = 0;
}


