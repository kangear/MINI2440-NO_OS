/*
*********************************************************************************************************
*                                         uC/OS-II ON MINI2440 BOARD
                                                ARM920T Port
*                                             ADS v1.2 Compiler                                                                                     
*                               (c) Copyright 2011,ZhenGuo Yao,ChengDu,Uestc
*                                           All Rights Reserved
*
* File : lcd.h
* By   : ZhenGuoYao
*********************************************************************************************************
*/
#ifndef LCD_H
#define LCD_H

#define LCD_WIDTH 	320
#define LCD_HEIGHT 	240
#define LCD_PIXCLOCK 	4

#define LCD_RIGHT_MARGIN 	0x44
#define LCD_LEFT_MARGIN 	0x04
#define LCD_HSYNC_LEN 		1

#define LCD_UPPER_MARGIN 	10
#define LCD_LOWER_MARGIN 	4
#define LCD_VSYNC_LEN 		1
#define LCD_CON5 		( (1 << 11)| (1<<0) | (1 << 8) | (1 << 6) | (1 << 9) | ( 1<< 10))

  /* color */
#define  clWhite	0xFFFF //��ɫ
#define  clBlack	0x0000 //��ɫ
#define  clDRed 	0x8000 //ǳ��ɫ
#define  clLRed 	0xF800 //���ɫ
#define  clDMagenta	0x8010 //����ɫ
#define  clLMagenta	0xF81F //����ɫ
#define  clGreen	0x07E0 //��ɫ
#define  clDBlue	0x0010 //����ɫ
#define  clLBlue	0x001F //����ɫ
#define  clDCyan	0x0410 //����ɫ
#define  clLCyan	0x07FF //����ɫ
#define  clDYellow	0x8400 //����ɫ
#define  clLYellow	0xFFE0 //����ɫ
#define  clDGray	0x8410 //����ɫ
#define  clLGray	0xF79E //����ɫ
#define  clLArgent	0xCE79 //����ɫ

#define LCD_TFT_XSIZE 	LCD_WIDTH	
#define LCD_TFT_YSIZE 	LCD_HEIGHT

#define SCR_XSIZE_TFT 	2*LCD_WIDTH
#define SCR_YSIZE_TFT 	2*LCD_HEIGHT

#define LCD_OFFSIZE    SCR_XSIZE_TFT-LCD_TFT_XSIZE 

void Lcd_T35_Init(void);

extern unsigned char shift480_640[];

void Lcd_ClearScr(unsigned short c);
void PutPixel(unsigned int x,unsigned int y, unsigned short c);
void GUI_Point(unsigned int x, unsigned int y, unsigned short c );
void Lcd_MoveViewPort(int vx,int vy);

void Glib_Rectangle(int x1,int y1,int x2,int y2,int color);
void Glib_FilledRectangle(int x1,int y1,int x2,int y2,int color);
void Glib_Line(int x1,int y1,int x2,int y2,int color);
void Paint_Bmp(int x0,int y0,int h,int l,unsigned char bm222p[]);
void drawCross(unsigned int x,unsigned int y,unsigned int color);

//================================
// �������
//================================
void Lcd_PutASCII(unsigned int x,unsigned int y,unsigned char ch,unsigned int c,unsigned int bk_c,unsigned int st); 
void Lcd_PutHZ(unsigned int x,unsigned int y,unsigned short int QW,unsigned int c,unsigned int bk_c,unsigned int st);  
void Lcd_printf(unsigned int x,unsigned int y,unsigned int c,unsigned int bk_c,unsigned int st,char *inputs);

#endif


