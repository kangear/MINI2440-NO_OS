/******************************************************************************
/* File：led_on_c.c
/* 功能：点亮一个LED
/******************************************************************************/
#define GPBCON      (*(volatile unsigned long *)0x56000010)
#define GPBDAT      (*(volatile unsigned long *)0x56000014)

int main()
{
    GPBCON = 0x00000400;    // 设置GPB5为输出口, 位[11:10]=0b01
    GPBDAT = 0x00000000;    // GPB5输出0，LED1点亮
    while(1);
    return 0;
}
