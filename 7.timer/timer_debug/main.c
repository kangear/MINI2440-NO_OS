/*
 * main.c
 *
 *  Created on: 2013-4-11
 *      Author: root
 */

#define	GPBCON		(*(volatile unsigned long *)0x56000010)
#define	GPBDAT		(*(volatile unsigned long *)0x56000014)

#define	GPB5_out	(1<<(5*2))
#define	GPB6_out	(1<<(6*2))
#define	GPB7_out	(1<<(7*2))
#define	GPB8_out	(1<<(8*2))

/*PWM & Timer registers*/
#define	TCFG0		(*(volatile unsigned long *)0x51000000)
#define	TCFG1		(*(volatile unsigned long *)0x51000004)
#define	TCON		(*(volatile unsigned long *)0x51000008)
#define	TCNTB0		(*(volatile unsigned long *)0x5100000c)
#define	TCMPB0		(*(volatile unsigned long *)0x51000010)
#define	TCNTO0		(*(volatile unsigned long *)0x51000014)

/*
 * Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}
 * {prescaler value} = 0~255
 * {divider value} = 2, 4, 8, 16
 * 本实验的Timer0的时钟频率=12MHz/(191+1)/(2)=31250Hz
 * 设置Timer0 0.5秒钟触发一次中断：
 */
void delay(int time)
{

	TCFG0  = 191;        // 预分频器0 = 191
	TCFG1  = 0x00;       // 选择2分频
	TCNTB0 = 31250/1000; // 1/1000秒钟定时
	TCMPB0 = 0;			 // 0阀值
	TCON   |= (1<<1);    // 手动更新
	TCON   = 0x09;       // 自动加载，清“手动更新”位，启动定时器0

	while(time--)
	{
		while(TCNTO0 != 0);  // 减到0就跳出
	}
}

void  wait(unsigned long dly)
{
	for(; dly > 0; dly--);
}

int main(void)
{
	unsigned long i = 0;

	GPBCON = GPB5_out|GPB6_out|GPB7_out|GPB8_out;		// 将LED1-4对应的GPB5/6/7/8四个引脚设为输出

	while(1){
		//wait(100000);
		delay(1000);
		GPBDAT = (~(i<<5));	 	// 根据i的值，点亮LED1-4
		if(++i == 16)
			i = 0;
	}

	return 0;
}
