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

int main()
{
    while(1)
    {
        // 从串口接收数据后，判断其是否数字或子母，若是则加1后输出
    	ReadAdc(0);
    }

    return 0;
}
