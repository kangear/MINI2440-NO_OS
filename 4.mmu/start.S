@*************************************************************************
@ File：head.S
@ 功能：设置SDRAM，将第二部分代码复制到SDRAM，设置页表，启动MMU，
@       然后跳到SDRAM继续执行
@*************************************************************************       

.text
.global _start
_start:
    ldr sp, =4096                       @ 设置栈指针，以下都是C函数，调用前需要设好栈
    bl  pre_lowlevel_init               @ 关闭WATCHDOG，否则CPU会不断重启
    bl  mem_ctrl_asm_init               @ 设置存储控制器(MMU要用这个)
    bl  create_page_table               @ 设置页表
    bl  mmu_init                        @ 启动MMUbn
    ldr sp, =4096                       @ 重设栈指针，指向内部SRAM顶端(使用虚拟地址)
	bl  main
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
 * 存储器控制器初始化函数
 */
#define BWSCON	0x48000000

mem_ctrl_asm_init:
	adrl	r0, SMRDATA			@这13个值的起始存储地址 这里的adrl不能简单的用ldr替代
	ldr		r1, =BWSCON			/* Bus Width Status Controller  存储控制器的13个寄存器的开始地址 */
	add     r2, r0, #13*4
0:
	ldr     r3, [r0], #4		@ 读取设置值，并让r0加4
	str     r3, [r1], #4		@ 将此值写入寄存器，并让r1加4
	cmp     r2, r0				@ 判断是否设置完所有13个寄存器
	bne     0b					@ 若没有写成，继续

	mov	pc, lr


	.ltorg
/* the literal pools origin */

SMRDATA:
    .word   0x22011110      @ BWSCON
    .word   0x00000700      @ BANKCON0
    .word   0x00000700      @ BANKCON1
    .word   0x00000700      @ BANKCON2
    .word   0x00000700      @ BANKCON3
    .word   0x00000700      @ BANKCON4
    .word   0x00000700      @ BANKCON5
    .word   0x00018005      @ BANKCON6
    .word   0x00018005      @ BANKCON7
    .word   0x008C07A3      @ REFRESH
    .word   0x000000B1      @ BANKSIZE
    .word   0x00000030      @ MRSRB6
    .word   0x00000030      @ MRSRB7
/* end_of mem_ctrl_asm_init*/
