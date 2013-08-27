@*************************************************************************
@ File：start.S
@ 功能：设置SDRAM，将程序复制到SDRAM，然后跳到SDRAM继续执行
@*************************************************************************       

.equ        MEM_CTL_BASE,       0x48000000
.equ        TEXT_BASE,         0x33F80000

.text
.global _start
_start:

	bl  pre_lowlevel_init               @ 关闭WATCHDOG，屏蔽中断
	bl  system_clock_init
	bl  mem_ctrl_asm_init               @ 设置存储控制器
	bl  nand_asm_init

relocate:				/* relocate U-Boot to RAM	    */
	adr	r0, _start		/* r0 <- current position of code   */
	ldr	r1, =TEXT_BASE		/* test if we run from flash or RAM */
	cmp	r0, r1			/* don't reloc during debug         */
	beq	stack_setup

	ldr sp, =1024*4                	 	@ 设置堆栈
    bl  copy_to_ram_from_nand	     	@ 复制代码到SDRAM中

stack_setup:
	bl	clear_bss                       @ 清理BSS段
	ldr sp, =0x33F80000                	@ 设置堆栈

	ldr	pc, _start_armboot              @ 跳到SDRAM中继续执行

_start_armboot:	.word main

@stack_setup:
@    ldr sp, =1024*4               		  @ 设置堆栈
@    bl  main

halt_loop:
    b   halt_loop

/*
 * 板级初始化预处理函数
 * 关看门狗、屏蔽中断
 */
pre_lowlevel_init:
	/* turn off the watchdog */
    #define pWTCON	0x53000000
    #define INTMSK	0x4A000008	/* Interrupt-Controller base addresses */
    #define INTSUBMSK	0x4A00001C
    #define CLKDIVN	0x4C000014	/* clock divisor register */

	ldr	r0, =pWTCON
	mov	r1, #0x0
	str	r1, [r0]

	/*
	 * mask all IRQs by setting all bits in the INTMR - default
	 */
	mov	r1, #0xffffffff
	ldr	r0, =INTMSK
	str	r1, [r0]

	ldr	r1, =0x7fff
	ldr	r0, =INTSUBMSK
	str	r1, [r0]

	mov	pc, lr
/*  end_of pre_lowlevel_init */

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

/*
 * 清BSS段
 */

/*
 * These are defined in the board-specific linker script.
 */
.globl _bss_start
_bss_start:
	.word __bss_start

.globl _bss_end
_bss_end:
	.word _end

clear_bss:
	ldr	r0, _bss_start		/* find start of bss segment        */
	ldr	r1, _bss_end		/* stop here                        */
	mov	r2, #0x00000000		/* clear                            */

clbss_l:str	r2, [r0]		/* clear loop...                    */
	add	r0, r0, #4
	cmp	r0, r1
	ble	clbss_l

	mov	pc, lr
/* end_of clear_bss */

/*
 * NAND FLASH初始化函数
 * TACLS:TWRPH0:TWRPH1 = 1:2:1, BUS_WIDTH_8
 */
#define S3C2440_NAND_BASE		0x4E000000
#define NFCONF_OFFSET    0x0
#define NFCONT_OFFSET    0x4

nand_asm_init:
			ldr		r0, =S3C2440_NAND_BASE
    		ldr		r1, =0x001210
			str   	r1, [r0, #NFCONF_OFFSET]

    		mov		r1, #0x3
			str   	r1, [r0, #NFCONT_OFFSET]

    		mov		pc, lr
/*end_of nand_asm_init*/

mem_ctrl_asm_init:
    @ 设置存储控制器以便使用SDRAM等外设

    mov r1,     #MEM_CTL_BASE       @ 存储控制器的13个寄存器的开始地址
    adrl    r2, mem_cfg_val         @ 这13个值的起始存储地址
    add r3,     r1, #52             @ 13*4 = 54
1:  
    ldr r4,     [r2], #4            @ 读取设置值，并让r2加4
    str r4,     [r1], #4            @ 将此值写入寄存器，并让r1加4
    cmp r1,     r3                  @ 判断是否设置完所有13个寄存器
    bne 1b                          @ 若没有写成，继续
    mov pc,     lr                  @ 返回


.align 4
mem_cfg_val:
    @ 存储控制器13个寄存器的设置值
    .long   0x22011110      @ BWSCON
    .long   0x00000700      @ BANKCON0
    .long   0x00000700      @ BANKCON1
    .long   0x00000700      @ BANKCON2
    .long   0x00000700      @ BANKCON3  
    .long   0x00000700      @ BANKCON4
    .long   0x00000700      @ BANKCON5
    .long   0x00018005      @ BANKCON6
    .long   0x00018005      @ BANKCON7
    .long   0x008C07A3      @ REFRESH
    .long   0x000000B1      @ BANKSIZE
    .long   0x00000030      @ MRSRB6
    .long   0x00000030      @ MRSRB7
