/*
 * mini2440 i2c最简裸机程序
 * 调试说明：
 *    调试脚本用最简的，不做SDRAM clock初始化，不关
 *    中断.mmu等等。
 *
 */

#include "s3c24xx.h"

int IIC_Ack =  0;

#define	GPB5_out	(1<<(5*2))
#define	GPB6_out	(1<<(6*2))
#define	GPB7_out	(1<<(7*2))
#define	GPB8_out	(1<<(8*2))

/********************************************************************************************************
 * Function : IIC read用到了，简单的延时是没有提升频率（12Mhz）用到的，如果提升频率，原来的原来的值要对应改变。
 * 以后可以直接用定时器来准确延时
 *当前版本：1.01
 *作者：kangear
 *完成时间：2013年4月15日
 *********************************************************************************************************/
void  wait(volatile unsigned long dly)
{
	for(; dly > 0; dly--);
}

/******************************************************************************
 * File：led_on_c.c
 * 功能：点亮一个LED
******************************************************************************/
#define GPBCON      (*(volatile unsigned long *)0x56000010)
#define GPBDAT      (*(volatile unsigned long *)0x56000014)

#define    I2CWrSlaAddr		    							0x00
#define    I2CWrMemAddr										0x01
#define    I2CSlaRdAddr										0x02
#define    I2CWrData										0x03
#define    I2CRdData										0x04
#define    I2CStop											0x05
#define    I2CPending										0x06

#define    I2CDisable										0x00
#define    I2CEnable										0x01
//===============================================================================================
//I2C寄存器定义
//===============================================================================================

//IICCON控制寄存器内容定义
#define IICBusAckEn											 (1 << 7)
#define IICBusAckDis										 (0 << 7)

#define TxClkSelPclk512										 (1 << 6)
#define TxClkSelPclk16										 (0 << 6)

#define TxRxIntEn											 (1 << 5)
#define TxRxIntDis											 (0 << 5)

#define TxRxIntPend											 (1 << 4)
#define TxRxIntNoPend									    ~(1 << 4)

#define TransClkVal 										 0x07	   //约400kbit/s
//IICSTAT控制寄存器内容定义
#define SlaveRecMode										 0x00
#define SlaveSendMode										 0x01
#define MasterRecMode										 0x02
#define MasterSendMode										 0x03

#define IICStop												 (0 << 5)
#define IICDataTrans										 (1 << 5)

#define IICDataDis									         (0 << 4)
#define IICDataEn										     (1 << 4)

#define ArbitateSucess									     (0 << 3)
#define ArbitateUnsucess									 (1 << 3)

#define	ClearStatFlag										 (0 << 2)  //检测到开始或停止条件时，改位清0
#define MachSlaveAddr										 (1 << 2)

#define CheckStartStop										 (0 << 1)
#define RecSlaAddr0									         (1 << 1)

#define IICRecAck											 (0 << 0)
#define IICRecNoAck											 (1 << 0)

//rIICLC控制寄存器内容定义
#define IICFilterDis									     (0 << 2)
#define IICFilterEn											 (1 << 2)

#define SDAOutDelay0										 0x00
#define SDAOutDelay5										 0x01
#define SDAOutDelay10										 0x02
#define SDAOutDelay15										 0x03


/********************************************************************************************************
 * Function : void I2C_Write(unsigned char SlaveAddr,unsigned short MemAddr,unsigned char Dat)
 * Description : S3C2440通过I2C主控制器向AT24C08写入数据
 * Inputs : SlaAddr：从机地址，MemAddr：单元地址，MemDat：数据，DatNum要写入数据的个数.
 * Outputs : None.
 *当前版本：1.01
 *作者：成都国嵌
 *完成时间：2011年5月10日
 *********************************************************************************************************/

void I2C_Write(unsigned char SlaAddr,unsigned short MemAddr,unsigned char *MemDat,unsigned char DatNum)
{
	unsigned char i;
	unsigned char HighAddr;
	unsigned char LowAddr;
	HighAddr = (unsigned char)((MemAddr & 0xff00) >> 8);
	LowAddr  = (unsigned char)(MemAddr & 0x00ff);
	IIC_Ack = 0;

	IICSTAT = (MasterSendMode << 6) | IICDataTrans | IICDataEn;
	IICDS   = SlaAddr;
	IICCON &= TxRxIntNoPend;
	while(IIC_Ack == 0);
	IIC_Ack = 0;

	IICDS = HighAddr;
	IICDS = LowAddr;
	IICCON &= TxRxIntNoPend;
	while(IIC_Ack == 0);
	IIC_Ack = 0;

	for(i = 0; i < DatNum; i++)
	{
		IICDS  = *MemDat;
		IICCON &= TxRxIntNoPend;
		while(IIC_Ack == 0);
		IIC_Ack = 0;
		MemDat++;
	}

	IICSTAT = (MasterSendMode << 6) | IICStop | IICDataEn;
	IICCON  = IICBusAckEn | TxRxIntEn | TransClkVal;
	wait(500);
}

/********************************************************************************************************
 * Function : void I2C_Read(unsigned char SlaveAddr,unsigned short MemAddr,unsigned char *Dat)
 * Description : S3C2440通过I2C主控制器向从AT24C08中读取数据
 * Inputs : SlaAddr：从机地址，MemAddr：单元地址，MemDat：数据，DatNum要读取数据的个数.
 * Outputs : None.
 *当前版本：1.01
 *作者：kangear
 *更新说明：把第一个IICDS 移到的IICSTAT的前边。不然第二次调试时会发送上次的残留值造成错误。
 *完成时间：2013年4月16日
 *********************************************************************************************************/
void I2C_Read(unsigned char SlaAddr,unsigned short MemAddr,unsigned char *MemDat,unsigned short DatNum)
{
	unsigned short i;
	unsigned char HighAddr;
	unsigned char LowAddr;
	HighAddr = (unsigned char)((MemAddr & 0xff00) >> 8);
	LowAddr  = (unsigned char)(MemAddr & 0x00ff);

	IIC_Ack = 0;
	IICDS   = SlaAddr;
	IICSTAT = (MasterSendMode << 6) | IICDataTrans | IICDataEn;
	IICCON &= TxRxIntNoPend;

	while(IIC_Ack == 0);
	IIC_Ack = 0;

	IICDS = HighAddr;
	IICDS = LowAddr;
	IICCON &= TxRxIntNoPend;
	while(IIC_Ack == 0);
	IIC_Ack = 0;

	IICSTAT = (MasterRecMode << 6) | IICDataTrans | IICDataEn;
	IICDS   = SlaAddr;
	IICCON &= TxRxIntNoPend;
	while(IIC_Ack == 0);
	IIC_Ack = 0;

	for(i = 0; i < (DatNum - 1); i++)
	{
		*MemDat = IICDS;
		wait(500);
		IICCON &= TxRxIntNoPend;
		while(IIC_Ack == 0);
		IIC_Ack = 0;
		*MemDat = IICDS;
		MemDat++;
	}

	*MemDat = IICDS;
	IICCON = IICBusAckDis | TxRxIntEn | TransClkVal;
	wait(500);
	*MemDat = IICDS;

	IICSTAT = (MasterRecMode << 6) | IICStop | IICDataEn;
	IICCON  = IICBusAckEn | TxRxIntEn | TransClkVal;
	wait(500);

}

/********************************************************************************************************
 * Function : void  BSP_I2C_Init(void)
 * Description : 初始化I2C接口
 * Inputs : None.
 * Outputs : None.
 *当前版本：0.1
 *作者：kangear
 *完成时间：2013年4月16日
**********************************************************************************************************/
void BSP_I2C_Init(void)
{
    GPEUP  |= 0xc000;       // 禁止内部上拉
    GPECON |= 0xa0000000;   // 选择引脚功能：GPE15:IICSDA, GPE14:IICSCL

    /* bit[7] = 1, 使能ACK
     * bit[6] = 0, IICCLK = PCLK/16
     * bit[5] = 1, 使能中断
     * bit[3:0] = 0xf, Tx clock = IICCLK/16
     * PCLK = 12MHz, IICCLK = 12MHz/16=750kHz, Tx Clock = IICCLK/(1+1) = 375kHz
     */
	IICCON  = (1 << 7) | (1 << 5) | (0x01); // 0xa1

	/* S3C24xx slave address = [7:1] 本例子中没有用到 */
//	IICADD  = 0x10;

	IICLC   = (1 << 2) | 0x01;

	/* I2C串行输出使能(Rx/Tx) */
	IICSTAT = (1 << 4);

	/* 开启IIC中断  */
	INTMSK &= ~(BIT_IIC);
}

/********************************************************************************************************
 * Function : void   BSP_I2CISR_Handler   (void)
 * Description : I2C中断处理函数
 * Inputs : None.
 * Outputs : None.
 *当前版本：0.1
 *作者：成都国嵌
 *完成时间：2011年5月10日
**********************************************************************************************************/
void  BSP_I2CISR_Handler   (void)
{
    //清I2C中断
    SRCPND = BIT_IIC;
    INTPND = BIT_IIC;
    IIC_Ack = 1;

}


int main(void)
{
	unsigned char RdDat[256];
	int RdMemAddr = 0;
	unsigned short RdDatNum = 10;
	unsigned char WrDat[10];
	unsigned long j = 0;
	for (j=0; j<256; j++)
	{
		RdDat[j] = 0;
		if( j < 10 )
			WrDat[j] = 9-j;
	}

	BSP_I2C_Init();

	I2C_Write(0xA0,0,WrDat,10);

	I2C_Read(0xA0,(RdMemAddr + 0x000),RdDat,RdDatNum);


	unsigned long i = 0;

    GPBCON = 0x00000400;    // 设置GPB5为输出口, 位[11:10]=0b01
    GPBDAT = 0x00000000;    // GPB5输出0，LED1点亮

	GPBCON = GPB5_out|GPB6_out|GPB7_out|GPB8_out;		// 将LED1-4对应的GPB5/6/7/8四个引脚设为输出

	while(1){
		wait(30000);
		GPBDAT = (~(i<<5));	 	// 根据i的值，点亮LED1-4
		if(++i == 16)
			i = 0;
	}

	return 0;
}
