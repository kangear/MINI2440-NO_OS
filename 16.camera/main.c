/*
main.c:
Copyright (C) 2011  zhenguoyao <yaozhenguo2006@126.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/.
*/
#include "./include/2440addr.h"
#include "./include/2440lib.h"
#include "./include/ov9650.h"
#include "./include/lcd.h"
#include "./include/mmu.h"


/*
 * 主函数
 */
int main(void)
{
	Port_Init();
	MMU_Init();
	enable_irq();
	Uart_Init();
 	Lcd_T35_Init();

	Uart_SendByte('\n');

	Uart_Printf("<***********************************************>\n");
	Uart_Printf("           S3C2440 Test Program cameratest\n");
	Uart_Printf("               made by kangear\n");
	Uart_Printf("<***********************************************>\n"); 
 
	ov9650_test();
}
