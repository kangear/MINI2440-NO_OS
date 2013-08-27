#include "s3c2410.h"
#include "serial.h"

#define	TXD0READY	(1<<2)
#define	RXD0READY	(1)

void init_uart(void)
{//初始化UART
	GPHCON |= 0xa0;		//GPH2,GPH3 used as TXD0,RXD0
	GPHUP	 = 0x0c;		//GPH2,GPH3内部上拉

	ULCON0	 = 0x03;		//8N1	
	UCON0	 = 0x05;		//查询方式
	UFCON0 = 0x00;		//不使用FIFO
	UMCON0 = 0x00;		//不使用流控
	UBRDIV0 = 12;		//波特率为57600

}

void putc(unsigned char c)
{
	while( ! (UTRSTAT0 & TXD0READY) );
	UTXH0 = c;
}

unsigned char getc(void)
{
#ifdef SERIAL_ECHO
	unsigned char ret;
#endif	
	while( ! (UTRSTAT0 & RXD0READY) );
	ret = URXH0;
#ifdef SERIAL_ECHO	
	if (ret == 0x0d || ret == 0x0a)
	{
		putc(0x0d);
		putc(0x0a);
	}
	else
	{
		putc(ret);		
	}
#endif	
	return ret;
}

