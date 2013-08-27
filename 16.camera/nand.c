
#include <string.h>
#include <stdlib.h>
#include "./include/2440addr.h"


#define NF_MECC_UnLock()    {rNFCONT&=~(1<<5);}
#define NF_MECC_Lock()      {rNFCONT|=(1<<5);}

#define NF_CMD(cmd)			{rNFCMD=cmd;}
#define NF_ADDR(addr)		{rNFADDR=addr;}	
#define NF_nFCE_L()			{rNFCONT&=~(1<<1);}
#define NF_nFCE_H()			{rNFCONT|=(1<<1);}
#define NF_RSTECC()			{rNFCONT|=(1<<4);}
#define NF_RDDATA() 		(rNFDATA)
#define NF_RDDATA8() 		((*(volatile unsigned char*)0x4E000010) )

#define NF_WRDATA(data) 	{rNFDATA=data;}

#define NF_WAITRB()    		{while(!(rNFSTAT&(1<<1)));} 
	   						 //wait tWB and check F_RNB pin.
// RnB Signal
#define NF_CLEAR_RB()    	{rNFSTAT |= (1<<2);}	// Have write '1' to clear this bit.
#define NF_DETECT_RB()    	{while(!(rNFSTAT&(1<<2)));}

#define TACLS		0	// 1-clk(0ns) 
#define TWRPH0		6	// 3-clk(25ns)
#define TWRPH1		0	// 1-clk(10ns)  //TACLS+TWRPH0+TWRPH1>=50ns

#define STATIC
STATIC void __RdPage512(unsigned char *buf)
{
	unsigned int i;
	for (i = 0; i < 512; i++) {
		buf[i] =  NF_RDDATA8();
	}
}



STATIC void Nand_Reset(void)
{
    volatile int i;
   
	NF_nFCE_L();
	NF_CLEAR_RB();
	for (i=0; i<10; i++);
	NF_CMD(0xFF);	//reset command
	NF_DETECT_RB();
	NF_nFCE_H();

}


STATIC unsigned int NF8_CheckId(void)
{
	int i;
	unsigned char Mid, Did, DontCare, id4th;  		

	NF_nFCE_L();
	
 	NF_CMD(0x90);
	NF_ADDR(0x0);
	for (i=0; i<100; i++);

	Mid 		= NF_RDDATA8();
	Did   		= NF_RDDATA8();
	DontCare 	= NF_RDDATA8();
	id4th 		= NF_RDDATA8();	

	NF_nFCE_H();
	
	switch(Did) {
	case 0x76:
		return 0;
	case 0xF1:
	case 0xD3:
	case 0xDA:
	case 0xDC:
		return 1;
	default:
		for(;;);
	}

}

STATIC int Nand_Init(void)
{
	rNFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4)|(0<<0);	
	rNFCONT = (0<<13)|(0<<12)|(0<<10)|(0<<9)|(0<<8)|(0<<6)|(0<<5)|(1<<4)|(1<<1)|(1<<0);
	rNFSTAT = 0;
	Nand_Reset();
	return NF8_CheckId();
}




STATIC int Nand_IsBadBlockPage512(unsigned int block)
{
    unsigned int 	Page;
    unsigned char 		BAD;
	
    Page = (block<<8);

    Nand_Reset();
     		
    NF_nFCE_L();
	NF_CLEAR_RB();
	NF_CMD(0x50);		 	    
  	NF_ADDR(5);	    
    NF_ADDR(Page&0xff);	 		
    NF_ADDR((Page>>8)&0xff);	
    NF_ADDR((Page>>16)&0xff);   
    NF_DETECT_RB();	 			

    BAD = NF_RDDATA8();
             
    NF_nFCE_H();

    return BAD != 0xff;
    
}

STATIC int Nand_IsBadBlockPage2048(unsigned int block)
{
    unsigned int 	Page;
    unsigned char 		BAD;
	
   	Page = block << 6;
   	NF_nFCE_L();
    NF_CLEAR_RB();
   	NF_CMD(0x00);
	NF_ADDR(0);
	NF_ADDR((2048>>8)&0xff);
    NF_ADDR(Page&0xff);
    NF_ADDR((Page>>8)&0xff);
    NF_ADDR((Page>>16)&0xff);
    NF_CMD(0x30);        
   	NF_DETECT_RB();

   	BAD = NF_RDDATA8();
             
   	NF_nFCE_H();    

    return BAD != 0xff;
}

STATIC int Nand_IsBadBlock(unsigned int block,int bLargeBlock)
{
	switch(bLargeBlock) {
	case 0:
		return Nand_IsBadBlockPage512(block);
	case 1:
		return Nand_IsBadBlockPage2048(block);
	}
	return 0;

}

STATIC int Nand_ReadSectorPage512(unsigned int sector, unsigned char *buffer)
{

    Nand_Reset();
    
	NF_nFCE_L();    

	NF_CLEAR_RB();
	NF_CMD(0x00);

    NF_ADDR(0x00);
	NF_ADDR(sector&0xff);
	NF_ADDR((sector>>8)&0xff);
	NF_ADDR((sector>>16)&0xff);
	 
	NF_DETECT_RB();	

    __RdPage512(buffer);

	NF_nFCE_H();    


    return 1;
}

STATIC int Nand_ReadSectorPage2048(unsigned int sector, unsigned char *buffer)
{
	unsigned int page, data_addr;

    Nand_Reset();
 
	NF_nFCE_L();    

	NF_CLEAR_RB();
	NF_CMD(0x00);						// Read command

	page = sector/4;
	data_addr = 512 *(sector%4);
	NF_ADDR(data_addr&0xff);
	NF_ADDR((data_addr>>8)&0xff);
	NF_ADDR(page&0xff);
	NF_ADDR((page>>8)&0xff);
	NF_ADDR((page>>16)&0xff);
	NF_CMD(0x30);
	
	 
	NF_DETECT_RB();	

    __RdPage512(buffer);

	NF_nFCE_H();    

   	return 1;
}


STATIC int Nand_ReadSector(unsigned int sector, unsigned char *buffer, int bLargeBlock)
{
	switch(bLargeBlock) {
	case 0:
		return Nand_ReadSectorPage512(sector, buffer);
	case 1:
		return Nand_ReadSectorPage2048(sector, buffer);
	}
	return 0;
}

int CopyProgramFromNand(void)
{
	const int bLargeBlock = Nand_Init();
	const unsigned StartSector = 0, StopSector = 5120;
	const unsigned byte_sector_shift = 9;
	const unsigned sector_block_shift = bLargeBlock ? 8 : 5;
	unsigned char *RAM = (unsigned char *)0x30000000;
	unsigned Sector, GoodSector;
	
	for (Sector = StartSector, GoodSector = Sector; Sector < StopSector; Sector ++, GoodSector++) {
	
		// begin of a block
		if ((GoodSector & ( (1 << sector_block_shift) - 1 )) == 0) {
			// found a good block
			for (;;) {
				if (!Nand_IsBadBlock(GoodSector >> sector_block_shift, bLargeBlock)) {
					// Is good Block
					break;
				}
				// try next block
				GoodSector += (1 << sector_block_shift);
			}
		}
		Nand_ReadSector(GoodSector, RAM + (Sector << byte_sector_shift ), bLargeBlock);
	}
	return 0;
}
