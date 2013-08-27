/*
*********************************************************************************************************
*                                         uC/OS-II ON MINI2440 BOARD
                                                ARM920T Port
*                                             ADS v1.2 Compiler                                                                                     
*                               (c) Copyright 2011,ZhenGuo Yao,ChengDu,Uestc
*                                           All Rights Reserved
*
* File : 2440LIB.C
* By   :FriendlyARM
* Modified By   : ZhenGuo Yao
*********************************************************************************************************
*/

#include "./include/2440addr.h"
#include "./include/2440lib.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*
*********************************************************************************************************
*                                         SYSTEN DELAY
*  time=0: adjust the Delay function by WatchDog timer.
*  time>0: the number of loop time
*  resolution of time is 100us 
*********************************************************************************************************
*/
//***************************[ SYSTEM ]***************************************************
void Delay(int time)
{
	unsigned int val = (PCLK>>3)/1000-1;
	
	rTCFG0 &= ~(0xff<<8);
	rTCFG0 |= 3<<8;			//prescaler = 3+1
	rTCFG1 &= ~(0xf<<12);
	rTCFG1 |= 0<<12;		//mux = 1/2

	rTCNTB3 = val;
	rTCMPB3 = val>>1;		// 50%
	rTCON &= ~(0xf<<16);
	rTCON |= 0xb<<16;		//interval, inv-off, update TCNTB3&TCMPB3, start timer 3
	rTCON &= ~(2<<16);		//clear manual update bit
	while(time--) {
		while(rTCNTO3>=val>>1);
		while(rTCNTO3<val>>1);
	};
}
/*
*********************************************************************************************************
*                                         PORTS
*********************************************************************************************************
*/
void Port_Init(void)
{
    //CAUTION:Follow the configuration order for setting the ports. 
    // 1) setting value(GPnDAT) 
    // 2) setting control register  (GPnCON)
    // 3) configure pull-up resistor(GPnUP)  

    //32bit data bus configuration  
    //*** PORT A GROUP
    //Ports  : GPA22 GPA21  GPA20 GPA19 GPA18 GPA17 GPA16 GPA15 GPA14 GPA13 GPA12  
    //Signal : nFCE nRSTOUT nFRE   nFWE  ALE   CLE  nGCS5 nGCS4 nGCS3 nGCS2 nGCS1 
    //Binary :  1     1      1  , 1   1   1    1   ,  1     1     1     1
    //Ports  : GPA11   GPA10  GPA9   GPA8   GPA7   GPA6   GPA5   GPA4   GPA3   GPA2   GPA1  GPA0
    //Signal : ADDR26 ADDR25 ADDR24 ADDR23 ADDR22 ADDR21 ADDR20 ADDR19 ADDR18 ADDR17 ADDR16 ADDR0 
    //Binary :  1       1      1      1   , 1       1      1      1   ,  1       1     1      1         
    rGPACON = 0x7fffff; 

    //**** PORT B GROUP
    //Ports  : GPB10    GPB9    GPB8    GPB7    GPB6     GPB5    GPB4   GPB3   GPB2     GPB1      GPB0
    //Signal : nXDREQ0 nXDACK0 nXDREQ1 nXDACK1 nSS_KBD nDIS_OFF L3CLOCK L3DATA L3MODE nIrDATXDEN Keyboard
    //Setting: OUTPUT  OUTPUT   OUTPUT  OUTPUT   OUTPUT   OUTPUT   OUTPUT OUTPUT OUTPUT   OUTPUT    OUTPUT 
    //Binary :   01  ,  01       01  ,   01      01   ,  01       01  ,   01     01   ,  01        01  
    rGPBCON = 0x155555;
    rGPBUP  = 0x7ff;     // The pull up function is disabled GPB[10:0]

    //*** PORT C GROUP
    //Ports  : GPC15 GPC14 GPC13 GPC12 GPC11 GPC10 GPC9 GPC8  GPC7   GPC6   GPC5 GPC4 GPC3  GPC2  GPC1 GPC0
    //Signal : VD7   VD6   VD5   VD4   VD3   VD2   VD1  VD0 LCDVF2 LCDVF1 LCDVF0 VM VFRAME VLINE VCLK LEND  
    //Binary :  10   10  , 10    10  , 10    10  , 10   10  , 10     10  ,  10   10 , 10     10 , 10   10
    rGPCCON = 0xaaaaaaaa;       
    rGPCUP  = 0xffff;     // The pull up function is disabled GPC[15:0] 

    //*** PORT D GROUP
    //Ports  : GPD15 GPD14 GPD13 GPD12 GPD11 GPD10 GPD9 GPD8 GPD7 GPD6 GPD5 GPD4 GPD3 GPD2 GPD1 GPD0
    //Signal : VD23  VD22  VD21  VD20  VD19  VD18  VD17 VD16 VD15 VD14 VD13 VD12 VD11 VD10 VD9  VD8
    //Binary : 10    10  , 10    10  , 10    10  , 10   10 , 10   10 , 10   10 , 10   10 ,10   10
    rGPDCON = 0xaaaaaaaa;       
    rGPDUP  = 0xffff;     // The pull up function is disabled GPD[15:0]

    //*** PORT E GROUP
    //Ports  : GPE15  GPE14 GPE13   GPE12   GPE11   GPE10   GPE9    GPE8     GPE7  GPE6  GPE5   GPE4  
    //Signal : IICSDA IICSCL SPICLK SPIMOSI SPIMISO SDDATA3 SDDATA2 SDDATA1 SDDATA0 SDCMD SDCLK I2SSDO 
    //Binary :  10     10  ,  10      10  ,  10      10   ,  10      10   ,   10    10  , 10     10  ,     
    //-------------------------------------------------------------------------------------------------------
    //Ports  :  GPE3   GPE2  GPE1    GPE0    
    //Signal : I2SSDI CDCLK I2SSCLK I2SLRCK     
    //Binary :  10     10  ,  10      10 
    rGPECON = 0xaaaaaaaa;       
    rGPEUP  = 0xffff;     // The pull up function is disabled GPE[15:0]

	
    //*** PORT F GROUP
    //Ports  : GPF7   GPF6   GPF5   GPF4      GPF3     GPF2  GPF1   GPF0
    //Signal : nLED_8 nLED_4 nLED_2 nLED_1 nIRQ_PCMCIA EINT2 KBDINT EINT0
    //Setting: Output Output Output Output    EINT3    EINT2 EINT1  EINT0
    //Binary :  01      01 ,  01     01  ,     10       10  , 10     10
    rGPFCON = 0x55aa;
    rGPFUP  = 0xff;     // The pull up function is disabled GPF[7:0]

    //*** PORT G GROUP
    //Ports  : GPG15 GPG14 GPG13 GPG12 GPG11    GPG10    GPG9     GPG8     GPG7      GPG6    
    //Signal : nYPON  YMON nXPON XMON  EINT19 DMAMODE1 DMAMODE0 DMASTART KBDSPICLK KBDSPIMOSI
    //Setting: nYPON  YMON nXPON XMON  EINT19  Output   Output   Output   SPICLK1    SPIMOSI1
    //Binary :   11    11 , 11    11  , 10      01    ,   01       01   ,    11         11
    //-----------------------------------------------------------------------------------------
    //Ports  :    GPG5       GPG4    GPG3    GPG2    GPG1    GPG0    
    //Signal : KBDSPIMISO LCD_PWREN EINT11 nSS_SPI IRQ_LAN IRQ_PCMCIA
    //Setting:  SPIMISO1  LCD_PWRDN EINT11   nSS0   EINT9    EINT8
    //Binary :     11         11   ,  10      11  ,  10        10
    rGPGCON = 0xff95ffba;
    rGPGUP  = 0xffff;    // The pull up function is disabled GPG[15:0]

    //*** PORT H GROUP
    //Ports  :  GPH10    GPH9  GPH8 GPH7  GPH6  GPH5 GPH4 GPH3 GPH2 GPH1  GPH0 
    //Signal : CLKOUT1 CLKOUT0 UCLK nCTS1 nRTS1 RXD1 TXD1 RXD0 TXD0 nRTS0 nCTS0
    //Binary :   10   ,  10     10 , 11    11  , 10   10 , 10   10 , 10    10
    rGPHCON = 0x2afaaa;
    rGPHUP  = 0x7ff;    // The pull up function is disabled GPH[10:0]

    // Added for S3C2440, DonGo
    //PORT J GROUP
    //Ports	:  GPJ12    GPJ11     GPJ10	GPJ9  GPJ8      GPJ7	GPJ6  GPJ5	GPJ4  GPJ3  GPJ2  GPJ1  GPJ0
    //Signal : CAMRESET CAMCLKOUT CAMHREF CAMVS CAMPCLKIN CAMD7 CAMD6 CAMD5 CAMD4 CAMD3 CAMD2 CAMD1 CAMD0
    //Setting: Out      Out      CAMHREF CAMVS CAMPCLKIN CAMD7 CAMD6 CAMD5 CAMD4 CAMD3 CAMD2 CAMD1 CAMD0
    //Binary : 01	  01        10      10    10        10    10    10    10    10    10    10    10
    //PU_OFF : 1	  0 		1	    1     1         1     1     1		1	1     1     1     1
    //---------------------------------------------------------------------------------------
    rGPJDAT = (1<<12)|(0<<11);
    rGPJCON = 0x016aaaa;
    rGPJUP	= ~((0<<12)|(1<<11));

    rGPJDAT = (0<<12)|(0<<11);
    rGPJCON = 0x016aaaa;
    rGPJUP	= 0x1fff;//~((1<<12)|(1<<11));

    //External interrupt will be falling edge triggered. 
    rEXTINT0 = 0x22222222;    // EINT[7:0]
    rEXTINT1 = 0x22222222;    // EINT[15:8]
    rEXTINT2 = 0x22222222;    // EINT[23:16]
}
/*
*********************************************************************************************************
*                                         UART
*********************************************************************************************************
*/

static int whichUart = 0;
void Uart_Init(void)
{
       rGPBCON = 0x015551;
       rGPBUP  = 0x7ff;
       rGPBDAT = 0x1e0;
       
       rGPHCON = 0x00faaa;                //使用UART0功能
       rGPHUP  = 0x7ff;
       rULCON0 = 0x3;                        //设置UART0无奇偶校验，一位停止位，8位数据
       rUCON0 = 0x245;  //PCLK为时钟源，接收和发送数据为查询或中断方式
       
       rUFCON0 = 0;                     //
       rUMCON0 = 0;                    //
       
       rUBRDIV0 = 26;                 //设置波特率，PCLK为50MHz，波特率为115.2kHz

}



void Uart_TxEmpty(int ch)
{
    if(ch==0)
        while(!(rUTRSTAT0 & 0x4)); //Wait until tx shifter is empty.
          
    else if(ch==1)
        while(!(rUTRSTAT1 & 0x4)); //Wait until tx shifter is empty.
        
    else if(ch==2)
        while(!(rUTRSTAT2 & 0x4)); //Wait until tx shifter is empty.
}

char Uart_Getch(void)
{
    if (whichUart == 0) {       
        while(!(rUTRSTAT0 & 0x1)); //Receive data ready
        return RdURXH0();
    }
    else if (whichUart == 1) {       
        while(!(rUTRSTAT1 & 0x1)); //Receive data ready
        return RdURXH1();
    }
    else if (whichUart == 2) {
        while(!(rUTRSTAT2 & 0x1)); //Receive data ready
        return RdURXH2();
    }
    return 0;
}

char Uart_GetKey(void)
{
    if (whichUart == 0) {       
        if(rUTRSTAT0 & 0x1)    //Receive data ready
            return RdURXH0();
        else
            return 0;
    }
    else if (whichUart == 1) {
        if(rUTRSTAT1 & 0x1)    //Receive data ready
            return RdURXH1();
        else
            return 0;
    }
    else if (whichUart == 2) {       
        if(rUTRSTAT2 & 0x1)    //Receive data ready
            return RdURXH2();
        else
            return 0;
    }
    return 0;    
}

void Uart_GetString (char *string)
{
    char *string2 = string;
    char c;
    while ((c = Uart_Getch())!='\r') {
        if (c == '\b') {
            if ((int)string2 < (int)string) {
                Uart_SendString("\b \b");
                string--;
            }
        } else {
            *string++ = c;
            Uart_SendByte(c);
        }
    }
    *string='\0';
    Uart_SendByte('\n');
}

void Uart_SendByte(int data)
{
    if (whichUart == 0) {
        if (data == '\n') {
            while(!(rUTRSTAT0 & 0x2));
            Delay(10);                 //because the slow response of hyper_terminal 
            WrUTXH0('\r');
        }
        while (!(rUTRSTAT0 & 0x2));   //Wait until THR is empty.
        Delay(10);
        WrUTXH0(data);
    }
    else if (whichUart == 1) {
        if (data == '\n') {
            while(!(rUTRSTAT1 & 0x2));
            Delay(10);                 //because the slow response of hyper_terminal 
            rUTXH1 = '\r';
        }
        while(!(rUTRSTAT1 & 0x2));   //Wait until THR is empty.
        Delay(10);
        rUTXH1 = data;
    }   
    else if (whichUart == 2) {
        if (data=='\n') {
            while(!(rUTRSTAT2 & 0x2));
            Delay(10);                 //because the slow response of hyper_terminal 
            rUTXH2 = '\r';
        }
        while(!(rUTRSTAT2 & 0x2));   //Wait until THR is empty.
        Delay(10);
        rUTXH2 = data;
    }       
}               


void Uart_SendString (char *pt)
{
    while(*pt)
        Uart_SendByte(*pt++);
}

void Uart0IsrInit(void)
{
       //interrupt 
       rSRCPND = 0x1<<28;
       rSUBSRCPND = 0x1;
       rINTPND = 0x1<<28;
       rINTSUBMSK &= ~(0x1);             //Open Uart0 receive interrupt mask
       rINTMSK &= ~(0x1<<28);            //Open Uart0 interrupt mask
}
/*
*********************************************************************************************************
*                                         MPLL
*********************************************************************************************************
*/
void ChangeMPllValue (int mdiv,int pdiv,int sdiv)
{
    rMPLLCON = (mdiv<<12) | (pdiv<<4) | sdiv;	
}


/*
*********************************************************************************************************
 *                                   使能全局中断
********************************************************************************************************
 */
void enable_irq(void) {
	asm volatile (
		"mrs r4,cpsr\n\t"
		"bic r4,r4,#0x80\n\t"
		"msr cpsr,r4\n\t"
		:::"r4"
	);
}
/*
*********************************************************************************************************
 *                                   关闭全局中断
********************************************************************************************************
 */
void disable_irq(void) {
	asm volatile (
		"mrs r4,cpsr\n\t"
		"orr r4,r4,#0x80\n\t"
		"msr cpsr,r4\n\t"
		:::"r4"
	);
}
/*
*********************************************************************************************************
*                                         UPLL
*********************************************************************************************************
*/
void ChangeUPllValue(int mdiv,int pdiv,int sdiv)
{
    rUPLLCON = (mdiv<<12) | (pdiv<<4) | sdiv;
}

/*
*********************************************************************************************************
*                                         BOARD LEDS
*********************************************************************************************************
*/
void LedOn (int num)
{
     unsigned int data;
     //Active is low.(LED On)
     // GPb8  GPb7   GPb6   GPB5
     //nLED_4 nLED_3 nLED_2 nLED_1
     switch (num) {
         case 0: data = 0x0f;break;
         case 1: data = 0x01;break;
         case 2: data = 0x02;break;
         case 3: data = 0x03;break;
         case 4: data = 0x04;break;
         default: data = 0x0f;
     }
     rGPBDAT = rGPBDAT & (~(data<<5));    
}

void LedOff (int num)
{
     unsigned int data;
     //Active is high.(LED Off)
     // GPb8  GPb7   GPb6   GPB5
     //nLED_4 nLED_3 nLED_2 nLED_1
     switch (num) {
         case 0: data = 0x0f;break;
         case 1: data = 0x01;break;
         case 2: data = 0x02;break;
         case 3: data = 0x03;break;
         case 4: data = 0x04;break;
         default : data = 0x0f;
         break;
     }
     rGPBDAT = rGPBDAT | (data<<5);    
}
/*
*********************************************************************************************************
*                                         BOARD BEEP
*********************************************************************************************************
*/
void BeepOn(void)
{
    rGPBDAT = rGPBDAT | 0x01;         
}
void BeepOff(void)
{
    rGPBDAT = rGPBDAT & (~0x01);
}


