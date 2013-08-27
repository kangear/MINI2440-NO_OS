/*
*********************************************************************************************************
*                                         uC/OS-II ON MINI2440 BOARD
                                                ARM920T Port
*                                             ADS v1.2 Compiler                                                                                     
*                               (c) Copyright 2011,ZhenGuo Yao,ChengDu,Uestc
*                                           All Rights Reserved
*
* File : 2440lib.h
* By   : ZhenGuoYao
*********************************************************************************************************
*/

#ifndef __2440lib_h__
#define __2440lib_h__


#define min(x1,x2) (((x1)<(x2))? (x1):(x2))
#define max(x1,x2) (((x1)>(x2))? (x1):(x2))

#define ONESEC0 (62500)	             //16us resolution, max 1.04 sec
#define ONESEC1 (31250)	             //32us resolution, max 2.09 sec
#define ONESEC2 (15625)	             //64us resolution, max 4.19 sec
#define ONESEC3 (7812)	             //128us resolution, max 8.38 sec
#define ONESEC4 (PCLK/128/(0xff+1))  //@60Mhz, 128*4us resolution, max 32.53 sec

#define EnterPWDN(clkcon) ((void (*)(int))0x20)(clkcon)

#define FCLK 400000000
#define HCLK (FCLK/4)
#define PCLK (HCLK/2)

void Delay (int time);
void Port_Init(void);
void Uart_Init(void);
void Uart_TxEmpty(int ch);
char Uart_Getch(void);
char Uart_GetKey(void);
void Uart_SendByte(int data);
void Uart_SendString(char *pt);
void Uart0IsrInit(void);
void Uart_Printf(char *fmt,...);

void ChangeUPllValue(int m,int p,int s);
void ChangeMPllValue(int m,int p,int s);
void enable_irq(void);
void disable_irq(void);
void LedOn (int num);
void LedOff (int num);
void BeepOn(void);
void BeepOff(void);


#endif  //__2440lib_h__
