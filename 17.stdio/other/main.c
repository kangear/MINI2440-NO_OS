#include "stdio.h"

void uart_init(void);

int main()
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
