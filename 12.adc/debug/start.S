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
