#include "uart.h"

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
    unsigned char c;
    unsigned char hello[] = "Hello uart!";
    int i = 0;
    uart_init();   // 波特率115200，8N1(8个数据位，无校验位，1个停止位)

    putc('K');

    for (i=0; i<12; i++)
    	putc(hello[i]);
    puts("\r\nshit");
    while(1)
    {
        // 从串口接收数据后，判断其是否数字或子母，若是则加1后输出
        c = getc();
        putc(c);
    }

    return 0;
}
