// ADCCON寄存器
#define PRESCALE_DIS        (0 << 14)
#define PRESCALE_EN         (1 << 14)
#define PRSCVL(x)           ((x) << 6)
#define ADC_INPUT(x)        ((x) << 3)
#define ADC_START           (1 << 0)
#define ADC_ENDCVT          (1 << 15)

// ADC
#define ADCCON      (*(volatile unsigned long *)0x58000000)	//ADC control
#define ADCTSC      (*(volatile unsigned long *)0x58000004)	//ADC touch screen control
#define ADCDLY      (*(volatile unsigned long *)0x58000008)	//ADC start or Interval Delay
#define ADCDAT0     (*(volatile unsigned long *)0x5800000c)	//ADC conversion data 0
#define ADCDAT1     (*(volatile unsigned long *)0x58000010)	//ADC conversion data 1
#define ADCUPDN     (*(volatile unsigned long *)0x58000014)	//Stylus Up/Down interrupt status

/*UART registers*/
#define ULCON0              (*(volatile unsigned long *)0x50000000)
#define UCON0               (*(volatile unsigned long *)0x50000004)
#define UFCON0              (*(volatile unsigned long *)0x50000008)
#define UMCON0              (*(volatile unsigned long *)0x5000000c)
#define UTRSTAT0            (*(volatile unsigned long *)0x50000010)
#define UTXH0               (*(volatile unsigned char *)0x50000020)
#define URXH0               (*(volatile unsigned char *)0x50000024)
#define UBRDIV0             (*(volatile unsigned long *)0x50000028)

#define TXD0READY   (1<<2)

/*
 * 使用查询方式读取A/D转换值
 * 输入参数：
 *     ch: 模拟信号通道，取值为0~7
 */
static int ReadAdc(int ch)
{
    // 选择模拟通道，使能预分频功能，设置A/D转换器的时钟 = PCLK/(49+1)
    ADCCON = PRESCALE_EN | PRSCVL(49) | ADC_INPUT(ch);

    // 清除位[2]，设为普通转换模式
    ADCTSC &= ~(1<<2);

    // 设置位[0]为1，启动A/D转换
    ADCCON |= ADC_START;

    // 当A/D转换真正开始时，位[0]会自动清0
    while (ADCCON & ADC_START);

    // 检测位[15]，当它为1时表示转换结束
    while (!(ADCCON & ADC_ENDCVT));

    // 读取数据
    return (ADCDAT0 & 0x3ff);
}

/*
 * 发送一个字符
 */
void putc(unsigned char c)
{
    /* 等待，直到发送缓冲区中的数据已经全部发送出去 */
    while (!(UTRSTAT0 & TXD0READY));

    /* 向UTXH0寄存器中写入数据，UART即自动将它发送出去 */
    UTXH0 = c;
}

void int2str(int num, char *str)
{
	int i = 0;
	int len = 0;
	char buf[10] = "";
	int temp = num < 0 ? -num : num;

	if (str == '\0')
	{
		return;
	}

	do	{
		buf[i++] = (temp % 10) + '0';
		temp /= 10;
	}	while (temp);

	len = num < 0 ? ++i : i;
	str[i] = 0; //此处是str的结束标志位，不要写成了buf[i] = 0，也不要写错了位置。

	while (1)
	{
		--i;
		if (buf[len - i - 1] == 0)
		{
			break;
		}
		str[i] = buf[len - i - 1];
	}

	if (i == 0)
	{
		str[i] = '-';
	}

}

void  wait(unsigned long dly)
{
	for(; dly > 0; dly--);
}

int main()
{
	unsigned char Adc_value[] = "ADC Value: ";
	unsigned char Adc[6] = {'0'};

	int i = 0;

    while(1)
    {
    	/*打印 "ADC Value: "*/
    	for (i=0; i<11; i++)
    	{
    	    putc(Adc_value[i]);
    	}

    	/*打印 ADC 值*/
    	int2str(ReadAdc(0), Adc);
		for (i=0; i<4; i++)
		{
		    putc(Adc[i]);
		}

		/*两个算一个换行回车*/
		putc('\n');
		putc('\r');

		wait(65536);
    }

    return 0;
}
