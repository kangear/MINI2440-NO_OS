/*
 *  armboot - Startup Code for ARM920 CPU-core
 *  kangear  2013-4-10
 */

.globl _start
_start:

    mov ip, lr
	bl	mem_ctrl_asm_init  /* 系统时钟初始化 */
@	ldr	sp, =1024*4     	 @ 设置堆栈，注意：不能大于4k, 因为现在可用的内存只有4K
@    bl  main                @ 调用C程序中的main函数


halt_loop:
    b   halt_loop


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
    .word   0x000000B2      @ BANKSIZE
    .word   0x00000030      @ MRSRB6
    .word   0x00000030      @ MRSRB7
/* end_of mem_ctrl_asm_init*/
