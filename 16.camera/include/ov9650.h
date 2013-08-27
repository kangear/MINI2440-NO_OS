#ifndef OV9650_H
#define OV9650_H

#define CLOCK_LOW()              (rGPEDAT&=(~(1<<14)))           //??堕??淇″??浣~N
#define CLOCK_HIGH()             (rGPEDAT|=(1<<14))                  //??堕??淇″??楂~X
#define DATA_LOW()                 (rGPEDAT&=(~(1<<15)))           //??版??淇″??浣~N
#define DATA_HIGH()                (rGPEDAT|=(1<<15))                  //??版??淇″??楂~X

#define udelay(x)               Delay(x/50)
#define mdelay(x)               Delay((x)*8)

extern unsigned short LCD_BUFFER[320][240];


void ov9650_test(void);

#endif

