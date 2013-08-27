/*
 * main.c
 *
 *  Created on: 2013-4-11
 *      Author: root
 */
#define	GPBCON		(*(volatile unsigned long *)0x56000010)
#define	GPBDAT		(*(volatile unsigned long *)0x56000014)

#define GPB0_tout	(2<<(0*2))

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
void Buzzer_init(void)
{
    TCFG0  = 191;        // 预分频器0 = 191
    TCFG1  = 0x00;       // 选择2分频
    TCNTB0 = 31250/10;   // freq为10，可以调整freq值来改变频率
    TCMPB0 = TCNTB0/2;   //占空比50％
    TCON   |= (1<<1);    // 手动更新
    TCON   = 0x09;       // 自动加载，清“手动更新”位，启动定时器0

}


int main(void)
{
	GPBCON |= GPB0_tout;                                // 将蜂鸣器对应的GPB0引脚设为输出TOUT。
	Buzzer_init();
	return 0;
}
