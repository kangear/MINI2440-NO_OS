#include "def.h"
#include "s3c24xx.h"

extern unsigned int *Rx_buffer;
extern unsigned int *Tx_buffer;


#define	GPB5_out	(1<<(5*2))
#define	GPB6_out	(1<<(6*2))
#define	GPB7_out	(1<<(7*2))
#define	GPB8_out	(1<<(8*2))

void  wait(volatile unsigned long dly)
{
	for(; dly > 0; dly--);
}

void  delay(void)
{
	wait(30000);
	wait(30000);
	wait(30000);
	wait(30000);
	wait(30000);
	wait(30000);
	wait(30000);
	wait(30000);
	wait(30000);
	wait(30000);
	wait(30000);
	wait(30000);
}

int led_init(void)
{
	GPBCON = GPB5_out|GPB6_out|GPB7_out|GPB8_out;		// 将LED1-4对应的GPB5/6/7/8四个引脚设为输出
	return 0;
}

int led_run(void)
{
	unsigned long i = 0;

	GPBCON = GPB5_out|GPB6_out|GPB7_out|GPB8_out;		// 将LED1-4对应的GPB5/6/7/8四个引脚设为输出
	while(1){
		delay();
		GPBDAT = (~(Rx_buffer[i]<<5));	 	// 根据i的值，点亮LED1-4
		if(++i == 16)
			break;
			//i = 0;
	}

	return 0;
}

int main(void)
{
	Test_SDI();
	led_run();
	while(1);
}
