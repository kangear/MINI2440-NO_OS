
#include "./include/mmulib.h"
#include "./include/mmu.h"


// 1) Only the section table is used. 
// 2) The cachable/non-cachable area can be changed by MMT_DEFAULT value.
//    The section size is 1MB.


/*
*********************************************************************************************
*                                       MMU_Init
*
* Description: This routine is used to initialize the MMU.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/


void MMU_Init(void)
{
    unsigned int i,j;
    //========================== IMPORTANT NOTE =========================
    //The current stack and code area can't be re-mapped in this routine.
    //If you want memory map mapped freely, your own sophiscated MMU
    //initialization code is needed.
    //===================================================================

    MMU_DisableDCache();
    MMU_DisableICache();

    //If write-back is used,the DCache should be cleared.
    for(i=0;i<64;i++)
        for(j=0;j<8;j++)
            MMU_CleanInvalidateDCacheIndex((i<<26)|(j<<5));
    MMU_InvalidateICache();

    #if 0
    //To complete MMU_Init() fast, Icache may be turned on here.
    MMU_EnableICache(); 
    #endif
    
    MMU_DisableMMU();
    MMU_InvalidateTLB();

    //MMU_SetMTT(U32 wVSAddr,U32 wVEAddr,U32 wPSAddr,U32 wAttrib)

    MMU_SetMTT(0x00000000,0x07f00000,0x30000000,RW_CNB);  // 1st Flash
    MMU_SetMTT(0x08000000,0x0ff00000,0x08000000,RW_CNB);  // 2nd Flash
    MMU_SetMTT(0x10000000,0x17f00000,0x10000000,RW_NCNB); //bank2
    MMU_SetMTT(0x18000000,0x1ff00000,0x18000000,RW_NCNB); //bank3
    MMU_SetMTT(0x20000000,0x27f00000,0x20000000,RW_NCNB); //bank4
    MMU_SetMTT(0x28000000,0x2ff00000,0x28000000,RW_NCNB); //bank5
    MMU_SetMTT(0x30000000,0x30f00000,0x30000000,RW_CB);   // 64MB SDRAM
    MMU_SetMTT(0x31000000,0x33e00000,0x31000000,RW_NCNB); //bank6-2
    MMU_SetMTT(0x33f00000,0x33f00000,0x33f00000,RW_CB);   //bank6-3
    MMU_SetMTT(0x38000000,0x3ff00000,0x38000000,RW_NCNB); //bank7
    
    MMU_SetMTT(0x40000000,0x40000000,0x40000000,RW_CNB);  // 4KB Internal SRAM
    MMU_SetMTT(0x48000000,0x5af00000,0x48000000,RW_NCNB); // SFR
    MMU_SetMTT(0x5b000000,0xfff00000,0x5b000000,RW_FAULT);//not used

    MMU_SetTTBase(MMUTT_SADDR);
    MMU_SetDomain(0x55555550|DOMAIN1_ATTR|DOMAIN0_ATTR); 
        //DOMAIN1: no_access, DOMAIN0,2~15=client(AP is checked)
    MMU_SetProcessId(0x0);
    MMU_EnableAlignFault();

    MMU_EnableMMU();
    MMU_EnableICache();
    MMU_EnableDCache(); //DCache should be turned on after MMU is turned on.
}    

/*
*********************************************************************************************
*                                       MMU_SetMTT
*
* Description: This routine sets MMU mapping table for a range for virtual addresses.
*
* Arguments  : wVSAddr - Virtual address start.
*              wVEAddr - Virtual address end.
*              wPSAddr - Physical address start.
*              wAttrib - MMU Attribute.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

void MMU_SetMTT(unsigned int wVSAddr,unsigned int wVEAddr,unsigned int wPSAddr,unsigned int wAttrib)
{
    unsigned int *pTT;
    unsigned int i,nSec;
    pTT=(unsigned int *)MMUTT_SADDR+(wVSAddr>>20);
    nSec=(wVEAddr>>20)-(wVSAddr>>20);
    for(i=0;i<=nSec;i++)*pTT++=wAttrib |(((wPSAddr>>20)+i)<<20);
}


/* ********************************************************************* */
