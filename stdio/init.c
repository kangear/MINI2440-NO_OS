#include "s3c2410.h"
 
void disable_watch_dog(void);
void memsetup(void);

/*上电后，WATCH DOG默认是开着的，要把它关掉 */
void disable_watch_dog()
{
	WTCON	= 0;
}


/**************************************************************************   
* 设置控制SDRAM的13个寄存器
* 使用位置无关代码
**************************************************************************/   
void memsetup(void)
{
	unsigned long *p = (unsigned long *)MEM_CTL_BASE;	
	p[0] = 0x22111110;		//BWSCON
	p[1] = 0x00000700;		//BANKCON0
	p[2] = 0x00000700;		//BANKCON1
	p[3] = 0x00000700;		//BANKCON2
	p[4] = 0x00000700;		//BANKCON3	
	p[5] = 0x00000700;		//BANKCON4
	p[6] = 0x00000700;		//BANKCON5
	p[7] = 0x00018005;		//BANKCON6
	p[8] = 0x00018005;		//BANKCON7
	p[9] = 0x008e07a3;		//REFRESH,HCLK=12MHz:0x008e07a3,HCLK=100MHz:0x008e04f4
	p[10] = 0x000000b2;		//BANKSIZE
	p[11] = 0x00000030;		//MRSRB6
	p[12] = 0x00000030;		//MRSRB7
}
