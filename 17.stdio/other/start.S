.global _start

_start:
	// 设置栈，以便调用c函数
	ldr	sp, =4096;

	// 汇编初始化时钟		
	bl system_clock_init

	// 调用main函数
	bl main							

halt:
	b halt

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
/* end_of system_clock_init*/
