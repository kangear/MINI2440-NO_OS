#include "stdio.h"

extern void uart_init(void);
extern int puts(const char*);

#define GPBCON      (*(volatile unsigned long *)0x56000010)
#define GPBDAT      (*(volatile unsigned long *)0x56000014)

int putchar(int c)
{
	if (c == '\n')
		putc('\r');
	
	if (c == '\b')
	{
		putc('\b');
		putc(' ');	
	}
			
	putc((char)c);
	
	return c;	
}

int puts(const char * s)
{
	while (*s)
		putchar(*s++);
		
	return 0;
}



int main()
{
	uart_init();
	puts("puts ok");
	printf("velloworld");
	GPBCON = 0x00000400;    // 设置GPB5为输出口, 位[11:10]=0b01
	GPBDAT = 0x00000000;    // GPB5输出0，LED1点亮
	while(1);
	return 0;
}

int main1()
{
	int a = 0;
	int b = 0;
	char *str = "hello world";

	uart_init();

	printf("%s\r\n",str);

	while (1)
	{
		printf("please enter two number: \r\n");
		scanf("%d %d", &a, &b);
		printf("\r\n");
		printf("the sum is: %d\r\n", a+b);
	}

	return 0;
}
