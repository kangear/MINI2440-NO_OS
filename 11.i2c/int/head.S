@******************************************************************************
@ File：head.S
@ 功能：初始化，设置中断模式、系统模式的栈，设置好中断处理函数
@******************************************************************************       
   
.extern     main
.text 
.global _start 
_start:
@******************************************************************************       
@ 异常向量，本程序中，除Reset和HandleIRQ外，其它异常都没有使用
@******************************************************************************       
    b   Reset

@ 0x04: 未定义指令中止模式的向量地址
HandleUndef:
    b   HandleUndef 
 
@ 0x08: 管理模式的向量地址，通过SWI指令进入此模式
HandleSWI:
    b   HandleSWI

@ 0x0c: 指令预取终止导致的异常的向量地址
HandlePrefetchAbort:
    b   HandlePrefetchAbort

@ 0x10: 数据访问终止导致的异常的向量地址
HandleDataAbort:
    b   HandleDataAbort

@ 0x14: 保留
HandleNotUsed:
    b   HandleNotUsed

@ 0x18: 中断模式的向量地址
    b   HandleIRQ

@ 0x1c: 快中断模式的向量地址
HandleFIQ:
    b   HandleFIQ

/*
 * These are defined in the board-specific linker script.
 */
.globl _bss_start
_bss_start:
	.word __bss_start

.globl _bss_end
_bss_end:
	.word __bss_end

Reset:                  
    ldr sp, =4096           @ 设置栈指针，以下都是C函数，调用前需要设好栈
    bl  pre_lowlevel_init   @ 关闭WATCHDOG，否则CPU会不断重启
    
    msr cpsr_c, #0xd2       @ 进入中断模式
    ldr sp, =3072           @ 设置中断模式栈指针

    msr cpsr_c, #0xdf       @ 进入系统模式
    ldr sp, =4096           @ 设置系统模式栈指针，
                            @ 其实复位之后，CPU就处于系统模式，
                            @ 前面的“ldr sp, =4096”完成同样的功能，此句可省略

    bl  clear_bss           @ 清除bss段，全局变量要用这个段。否则全局变量无法改变。

@    bl  init_led            @ 初始化LED的GPIO管脚
@    bl  init_irq            @ 调用中断初始化函数，在init.c中
    msr cpsr_c, #0x5f       @ 设置I-bit=0，开IRQ中断
    
    ldr lr, =halt_loop      @ 设置返回地址
    ldr pc, =main           @ 调用main函数
halt_loop:
    b   halt_loop

HandleIRQ:
    sub lr, lr, #4                  @ 计算返回地址
    stmdb   sp!,    { r0-r12,lr }   @ 保存使用到的寄存器
                                    @ 注意，此时的sp是中断模式的sp
                                    @ 初始值是上面设置的3072
    
    ldr lr, =int_return             @ 设置调用ISR即EINT_Handle函数后的返回地址  
    ldr pc, =BSP_I2CISR_Handler     @ 调用中断服务函数，在main.c中
int_return:
    ldmia   sp!,    { r0-r12,pc }^  @ 中断返回, ^表示将spsr的值复制到cpsr

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
/* end_of pre_lowlevel_init */

/*
 * 清BSS段
 */
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
