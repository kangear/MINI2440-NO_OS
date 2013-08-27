/*
 * leds.c: 循环点亮4个LED
 * 属于第二部分程序，此时MMU已开启，使用虚拟地址
 */ 

/*
 * MMU开过后，物理地址就不能用了 可以替换试试
 * 也可以关闭mmu，换成物理地址,感受led速度不同
 * 开的MMU时也开了Cache,TLB速度是它们提高的
 */
#define GPBCON      (*(volatile unsigned long *)0xA0000010)     // 物理地址0x56000010 虚拟地址0xA0000010
#define GPBDAT      (*(volatile unsigned long *)0xA0000014)     // 物理地址0x56000014 虚拟地址0xA0000014

#define GPB5_out    (1<<(5*2))
#define GPB6_out    (1<<(6*2))
#define GPB7_out    (1<<(7*2))
#define GPB8_out    (1<<(8*2))

/*
 * wait函数加上“static inline”是有原因的，
 * 这样可以使得编译leds.c时，wait嵌入main中，编译结果中只有main一个函数。
 * 于是在连接时，main函数的地址就是由连接文件指定的运行时装载地址。
 * 而连接文件mmu.lds中，指定了leds.o的运行时装载地址为0xB4004000，
 * 这样，head.S中的“ldr pc, =0xB4004000”就是跳去执行main函数。
 */
static inline void wait(volatile unsigned long dly)
{
    for(; dly > 0; dly--);
}

int main(void)
{
    unsigned long i = 0;
    
    // 将LED1-4对应的GPB5/6/7/8四个引脚设为输出
    GPBCON = GPB5_out|GPB6_out|GPB7_out|GPB8_out;       

    while(1){
        wait(3000000);
        GPBDAT = (~(i<<5));     // 根据i的值，点亮LED1-4
        if(++i == 16)
            i = 0;
    }

    return 0;
}

