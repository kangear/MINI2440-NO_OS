/* WOTCH DOG register */
#define 	WTCON				(*(volatile unsigned long *)0x53000000)

/* SDRAM regisers */
#define 	MEM_CTL_BASE		0x48000000
#define 	SDRAM_BASE		0x30000000

/* NAND Flash registers */
#define NFCONF				(*(volatile unsigned int  *)0x4e000000)
#define NFCMD				(*(volatile unsigned char *)0x4e000004)
#define NFADDR				(*(volatile unsigned char *)0x4e000008)
#define NFDATA				(*(volatile unsigned char *)0x4e00000c)
#define NFSTAT				(*(volatile unsigned char *)0x4e000010)

/*GPIO registers*/
#define	GPBCON			(*(volatile unsigned long *)0x56000010)
#define	GPBDAT				(*(volatile unsigned long *)0x56000014)

#define	GPHCON			(*(volatile unsigned long *)0x56000070)
#define	GPHDAT				(*(volatile unsigned long *)0x56000074)
#define	GPHUP				(*(volatile unsigned long *)0x56000078)



/*UART registers*/
#define	ULCON0				(*(volatile unsigned long *)0x50000000)
#define	UCON0				(*(volatile unsigned long *)0x50000004)
#define	UFCON0			 	(*(volatile unsigned long *)0x50000008)
#define	UMCON0			(*(volatile unsigned long *)0x5000000c)
#define	UTRSTAT0			(*(volatile unsigned long *)0x50000010)
#define	UTXH0			 	(*(volatile unsigned char *)0x50000020)
#define	URXH0			 	(*(volatile unsigned char *)0x50000024)
#define	UBRDIV0			(*(volatile unsigned long *)0x50000028)
