/*
 *  armboot - Startup Code for ARM920 CPU-core
 *  kangear  2013-4
 */

.globl _start
_start:

    bl	pre_lowlevel_init   @ 关看门狗
@	bl	system_clock_init  /* 系统时钟初始化 */
	ldr	sp, =1024*4     	@ 设置堆栈，注意：不能大于4k, 因为现在可用的内存只有4K
    bl  main                @ 调用C程序中的main函数


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
