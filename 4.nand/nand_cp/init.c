
/* WOTCH DOG register */
#define 	WTCON				(*(volatile unsigned long *)0x53000000)
#define	CLKDIVN		(*(volatile unsigned long *)0x4c000014)
#define	MPLLCON		(*(volatile unsigned long *)0x4c000004)

/* SDRAM regisers */
#define S3C2440_MPLL_400MHZ     ((0x7f<<12)|(0x02<<4)|(0x01))
#define 	MEM_CTL_BASE		0x48000000
void disable_watch_dog();
void memsetup();
void clock_init(void);
/*上电后，WATCH DOG默认是开着的，要把它关掉 */
void disable_watch_dog()
{
	WTCON	= 0;
}

void clock_init(void)
{
    // LOCKTIME = 0x00ffffff;   // 使用默认值即可
    CLKDIVN  = 0x05;            // FCLK:HCLK:PCLK=1:4:8, HDIVN=2,PDIVN=1

    /* 如果HDIVN非0，CPU的总线模式应该从“fast bus mode”变为“asynchronous bus mode” */
__asm__(
    "mrc    p15, 0, r1, c1, c0, 0\n"        /* 读出控制寄存器 */ 
    "orr    r1, r1, #0xc0000000\n"          /* 设置为“asynchronous bus mode” */
    "mcr    p15, 0, r1, c1, c0, 0\n"        /* 写入控制寄存器 */
    );

   MPLLCON = S3C2440_MPLL_400MHZ;  /* 现在，FCLK=400MHz,HCLK=100MHz,PCLK=50MHz */
}
/* 设置控制SDRAM的13个寄存器 */
void memsetup()
{
	int 	i = 0;
	unsigned long *p = (unsigned long *)MEM_CTL_BASE;

    /* SDRAM 13个寄存器的值 */
    unsigned long  const    mem_cfg_val[]={ 0x22011110,     //BWSCON
                                            0x00000700,     //BANKCON0
                                            0x00000700,     //BANKCON1
                                            0x00000700,     //BANKCON2
                                            0x00000700,     //BANKCON3  
                                            0x00000700,     //BANKCON4
                                            0x00000700,     //BANKCON5
                                            0x00018005,     //BANKCON6
                                            0x00018005,     //BANKCON7
                                            0x008C07A3,     //REFRESH
                                            0x000000B1,     //BANKSIZE
                                            0x00000030,     //MRSRB6
                                            0x00000030,     //MRSRB7
                                    };

	for(; i < 13; i++)
		p[i] = mem_cfg_val[i];
}
