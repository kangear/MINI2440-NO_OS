#include "def.h"
#include "s3c24xx.h"


#define	GPBCON		(*(volatile unsigned long *)0x56000010)
#define	GPBDAT		(*(volatile unsigned long *)0x56000014)

#define	GPB5_out	(1<<(5*2))
#define	GPB6_out	(1<<(6*2))
#define	GPB7_out	(1<<(7*2))
#define	GPB8_out	(1<<(8*2))

void  wait(volatile unsigned long dly)
{
	for(; dly > 0; dly--);
}

int led_run(void)
{
	unsigned long i = 0;

	GPBCON = GPB5_out|GPB6_out|GPB7_out|GPB8_out;		// 将LED1-4对应的GPB5/6/7/8四个引脚设为输出

	while(1){
		wait(30000);
		GPBDAT = (~(i<<5));	 	// 根据i的值，点亮LED1-4
		if(++i == 16)
			i = 0;
	}

	return 0;
}


#define LCD_N35 //NEC 256K色240*320/3.5英寸TFT真彩液晶屏,每一条水平线上包含240个像素点，共有320条这样的线

#if defined(LCD_N35)

#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define LCD_PIXCLOCK 4

#define LCD_RIGHT_MARGIN 0x44//68//1
#define LCD_LEFT_MARGIN 0x04//2//2
#define LCD_HSYNC_LEN 1

#define LCD_UPPER_MARGIN 10//10//4
#define LCD_LOWER_MARGIN 4//18//18
#define LCD_VSYNC_LEN 1

#endif

void TFT_LCD_Test(void);

#define LCD_XSIZE  LCD_WIDTH
#define LCD_YSIZE  LCD_HEIGHT
#define SCR_XSIZE  LCD_WIDTH
#define SCR_YSIZE  LCD_HEIGHT

volatile static unsigned short LCD_BUFFER[SCR_YSIZE][SCR_XSIZE];  //定义320行，240列的数组，用于存放显示数据

extern unsigned char sunflower_320x240[];
#define ADC_FREQ 2500000
//#define ADC_FREQ   1250000

volatile U32 preScaler;




static void cal_cpu_bus_clk(void);
void Set_Clk(void);

/*************************************************
Function name: delay
Parameter    : times
Description	 : 延时函数
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/
void delay(int times)
{
    int i,j;
    for(i=0;i<times;i++)
       for(j=0;j<400;j++);
}
/*************************************************
Function name: Main
Parameter    : void
Description	 : 主功能函数
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/
int main(void)
{

    int Scom=0;
//    MMU_Init();
//    Set_Clk();
 	//SERAL  GPIO
// 	GPHCON &= ~(0X0F << 4);
// 	GPHCON |= (0X0A << 4);


//    Uart_Init(0,115200);
//    Uart_Select(Scom);
//    Uart_Printf("\nHello World!\n");

//    led_run();
    TFT_LCD_Test();

	return 0;

}


/*************************************************
Function name: Pait_Bmp
Parameter    : void
Description	 : 在LCD屏幕上指定坐标点画一个指定大小的图片
Return		 : void
Argument     : int x0,int y0,int h,int l,const unsigned char *bmp
Autor & date : Daniel
**************************************************/
static void Pait_Bmp(int x0,int y0,int h,int l,const unsigned char *bmp)
{
	int x,y;
	U32 c;
	int p = 0;

    for( y = 0 ; y < l ; y++ )
    {
    	for( x = 0 ; x < h ; x++ )
    	{
    		c = bmp[p+1] | (bmp[p]<<8) ;

			if ( ( (x0+x) < SCR_XSIZE) && ( (y0+y) < SCR_YSIZE) )
				LCD_BUFFER[y0+y][x0+x] = c ;

    		p = p + 2 ;
    	}
    }
}


/*************************************************
Function name: Lcd_ClearScr
Parameter    : void
Description	 : 240×320 TFT LCD全屏填充特定颜色单元或清屏
Return		 : void
Argument     : U16
Autor & date : Daniel
**************************************************/
static void Lcd_ClearScr( U16 c)
{
	unsigned int x,y ;

    for( y = 0 ; y < SCR_YSIZE ; y++ )
    {
    	for( x = 0 ; x < SCR_XSIZE ; x++ )
    	{
			LCD_BUFFER[y][x] = c ;
    	}
    }
}


/*************************************************
Function name: Lcd_EnvidOnOff
Parameter    : void
Description	 : LCD视频和控制信号输出或者停止，1开启视频显示数据输出
Return		 : void
Argument     : int
Autor & date : Daniel
**************************************************/
static void Lcd_EnvidOnOff(int onoff)
{
    if(onoff==1)
	LCDCON1|=1; // ENVID=ON
    else
	LCDCON1 =LCDCON1 & 0x3fffe; // ENVID Off
}


/*************************************************
Function name: Lcd_Port_Init
Parameter    : void
Description	 : 240*320 TFT LCD数据和控制端口初始化
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/
static void Lcd_Port_Init( void )
{
    GPCUP=0xffffffff; // Disable Pull-up register
    GPCCON=0xaaaa02a8; //Initialize VD[7:0],VM,VFRAME,VLINE,VCLK

    GPDUP=0xffffffff; // Disable Pull-up register
    GPDCON=0xaaaaaaaa; //Initialize VD[15:8]
}


/*************************************************
Function name: LCD_Init
Parameter    : void
Description	 : 240×320 TFT LCD功能模块初始化
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/
static void LCD_Init(void)
{
#define	M5D(n)	((n)&0x1fffff)
#define LCD_ADDR ((U32)LCD_BUFFER)

	/*bit[17:8](4:CLKVAL);bit[6:5](11:TFT LCD panel);bit[4:1](1100:16 bpp for TFT)*/
	LCDCON1 = (LCD_PIXCLOCK << 8) | (3 <<  5) | (12 << 1) ;

	/*bit[31:24](1:VBPD);bit[23:14](320-1:行数);bit[13:6](5:VFPD);bit[5:0](1:VSPW)*/
   	LCDCON2 = (LCD_UPPER_MARGIN << 24) | ((LCD_HEIGHT - 1) << 14) | (LCD_LOWER_MARGIN << 6) | (LCD_VSYNC_LEN << 0);

   	/*bit[25:19](36:HBPD);bit[18:8](240-1:列数);bit[7:0](19:HFPD)*/
   	LCDCON3 = (LCD_RIGHT_MARGIN << 19) | ((LCD_WIDTH  - 1) <<  8) | (LCD_LEFT_MARGIN << 0);

   	/*bit[15:8](13:MVAL,只有当LCDCON1 bit[7]MMODE=1才有效);bit[7:0](5:HSPW)*/
   	LCDCON4 =  (LCD_HSYNC_LEN << 0);

#if !defined(LCD_CON5)
	/*bit[11](5:6:5 Format);bit[9](VLINE/HSYNC polarity inverted);bit[8](VFRAME/VSYNC inverted)
	  bit[3](Enalbe PWERN signal),bit[1](half-word swap control bit)*/
#    define LCD_CON5 ((1<<11) | (1 << 9) | (1 << 8) | (1 << 3) | (1 << 0))
#endif
    LCDCON5   =  LCD_CON5;

	/*
	LCDBANK: 视频帧缓冲区内存地址30-22位
	LCDBASEU: 视频帧缓冲区的开始地址21-1位
	LCDBASEL: 视频帧缓冲区的结束地址21-1位
	*/
	/*bit[29:21]:LCDBANK,bit[20:0]:LCDBASEU*/
    LCDSADDR1 = ((LCD_ADDR >> 22) << 21) | ((M5D(LCD_ADDR >> 1)) <<  0);

    /*bit[20:0]:LCDBASEL*/
    LCDSADDR2 = M5D((LCD_ADDR + LCD_WIDTH * LCD_HEIGHT * 2) >> 1);


    /*PAGEWIDTH:虚拟屏幕一行的字节数，如果不使用虚拟屏幕，设置为实际屏幕的行字节数
	  OFFSIZE:虚拟屏幕左侧偏移的字节数，如果不使用虚拟屏幕，设置为0
	*/
	/*bit[21:11]:OFFSIZE; bit[10:0]:PAGEWIDTH*/
    LCDSADDR3 = LCD_WIDTH;

	/*屏蔽中断*/
    LCDINTMSK |= 3;
  	TCONSEL   &= (~7);

	/*disable调色板*/
   	TPAL     = 0x0;

   	/*禁止LPC3600/LCC3600模式*/
   	TCONSEL &= ~((1<<4) | 1);


}

/*************************************************
Function name: TFT_LCD_Testt
Parameter    : void
Description	 : mini2440 LCD测试
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/
void TFT_LCD_Test(void)
{
	U32 i;

	////Uart_Printf("\nTest TFT LCD 240x320!\n");


    Lcd_Port_Init();
    LCD_Init();
    Lcd_EnvidOnOff(1);		//turn on vedio

	//Uart_Printf("LCD clear screen!\n");

	/*红(255:0:0);绿(0:255:0);蓝(0:0:255);黑(0:0:0);白(255,255,255)*/

	/*在屏幕上显示三基色*/

		Lcd_ClearScr( (0x00<<11) | (0x00<<5) | (0x00)  )  ;		//clear screen black
		delay(10000);

		Lcd_ClearScr((0x1f<<11) | (0x00<<5) | (0x00));			//red
		delay(10000);
		Lcd_ClearScr((0x00<<11) | (0x3f<<5) | (0x00));			//green
		delay(10000);
		Lcd_ClearScr((0x00<<11) | (0x00<<5) | (0x1f));			//blue
		delay(10000);

		Lcd_ClearScr( (0x1f<<11) | (0x3f<<5) | (0x1f)  )  ;		//clear screen white
		delay(10000);




	Lcd_ClearScr(0xffff);		//fill all screen with some color

	/*显示一副图片在屏幕上*/

	Pait_Bmp(0, 0, 320, 240, sunflower_320x240);
	delay(10000);
	delay(10000);
	delay(10000);

   	//Uart_Printf( "LCD clear screen is finished!\n" );

  //  Lcd_EnvidOnOff(0);		//turn off vedio
}



