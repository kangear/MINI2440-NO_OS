@******************************************************************************
@ File：start.S
@ 功能：启动代码
@******************************************************************************

.extern     main
.text
.global _start
_start:
Reset:
    ldr sp, =4096           @ 设置栈指针，以下都是C函数，调用前需要设好栈
    bl  pre_lowlevel_init   @ 关闭WATCHDOG，否则CPU会不断重启
    bl  uart_asm_init       @ 汇编初始化uart
    ldr lr, =halt_loop      @ 设置返回地址
    ldr pc, =main           @ 调用main函数
halt_loop:
    b   halt_loop

/*
 * 关看门狗
 */
pre_lowlevel_init:
	/* turn off the watchdog */
    #define pWTCON	0x53000000

	ldr	r0, =pWTCON
	mov	r1, #0x0
	str	r1, [r0]

	mov	pc, lr
/*$:  end_of pre_lowlevel_init */

/*
 * 串口初始化函数
 * 波特率：9600
 */
#define S3C24X0_GPIO_BASE		0x56000000
#define GPHCON_OFFSET 0x70
#define UFCON_OFFSET  0x08 /* S3C2410_UFCON */
#define UMCON_OFFSET  0x0c /* S3C2410_UMCON */
#define ULCON_OFFSET  0x00 /* S3C2410_ULCON */
#define UCON_OFFSET   0x04 /* S3C2410_UCON  */
#define UBRDIV_OFFSET 0x28 /* S3C2410_UBRDIV */

#define S3C24X0_UART_BASE		0x50000000
#define UART_CONSOLE_BASE S3C24X0_UART_BASE

uart_asm_init:
	ldr	r0, =S3C24X0_GPIO_BASE
	ldr	r1, =0x00faaa
	str   	r1, [r0, #GPHCON_OFFSET]   // GPH2,GPH3用作TXD0,RXD0

	ldr	r0, =UART_CONSOLE_BASE
	mov	r1, #0x0
	str	r1, [r0, #UFCON_OFFSET]        // 不使用FIFO
	str	r1, [r0, #UMCON_OFFSET]        // 不使用流控

	mov	r1, #0x3
	str	r1, [r0, #ULCON_OFFSET]        // 8N1(8个数据位，无较验，1个停止位)

	ldr	r1, =0x245
	str	r1, [r0, #UCON_OFFSET]         // 查询方式，UART时钟源为PCLK

	ldr	r1, =0x4D
	str	r1, [r0, #UBRDIV_OFFSET]       // 波特率为9600(UBRDIV为0x4d)  波特率为115200(UBRDIV为0x1a)

	mov	pc, lr
 /* uart_asm_init */

