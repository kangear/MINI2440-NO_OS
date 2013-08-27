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
    bl  system_clock_init   @ 设置MPLL，改变FCLK、HCLK、PCLK
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
 * 系统时钟初始化函数
 * S3C2440: FCLK:HCLK:PCLK = 1:4:8(FCLK is 405 MHz)
 */
#define CLKDIVN	0x4C000014
#define CLK_CTL_BASE	0x4C000000
#define MDIV_405	0x7f << 12
#define PSDIV_405	0x21
#define MDIV  MDIV_405
#define PSDIV PSDIV_405
#define CLKDIV 0x5   /* FCLK:HCLK:PCLK = 1:4:8 */

system_clock_init:
	ldr	r0, =CLKDIVN
	mov	r1, #CLKDIV
	str	r1, [r0]
	/*
     * 设置arm920t为异步时钟模式
     *
     * 复位时，arm920t处于快速总线时钟模式，该模式下
     * core和AMBA都由BCLK控制，设置为异步模式后，core
     * 由FCLK控制
     */
	mrc	p15, 0, r1, c1, c0, 0
	orr	r1, r1, #0xc0000000
	mcr	p15, 0, r1, c1, c0, 0

	mov	r1, #CLK_CTL_BASE
	mov	r2, #MDIV
	add	r2, r2, #PSDIV
	str	r2, [r1, #0x04]		/* MPLLCON  */

	mov	pc, lr
