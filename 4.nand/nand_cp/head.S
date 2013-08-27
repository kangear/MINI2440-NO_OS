@******************************************************************************
@ File：head.s
@ 功能：设置SDRAM，将程序复制到SDRAM，然后跳到SDRAM继续执行
@******************************************************************************       
  
.text
.global _start
_start:
                                            @函数disable_watch_dog, memsetup, init_nand, nand_read_ll在init.c中定义
            ldr     sp, =4096               @设置堆栈 
            bl      disable_watch_dog       @关WATCH DOG
			bl      clock_init
            bl      memsetup                @初始化SDRAM
            bl      nand_asm_init           @初始化NAND Flash
			bl      copy_to_ram_from_nand
            ldr     sp, =0x34000000         @设置栈
            ldr     lr, =halt_loop          @设置返回地址
            ldr		pc, _start_armboot		@b指令和bl指令只能前后跳转32M的范围，所以这里使用向pc赋值的方法进行跳转

_start_armboot:	.word main

halt_loop:
            b       halt_loop

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
