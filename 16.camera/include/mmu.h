/**************************************************************************
*                                                                         *
*   PROJECT     : ARM port for uC/OS-II                                   *
*                                                                         *
*   MODULE      : MMU.h                                                   *
*                                                                         *
*   AUTHOR      : Michael Anburaj                                         *
*                 URL  : http://geocities.com/michaelanburaj/             *
*                 EMAIL: michaelanburaj@hotmail.com                       *
*                                                                         *
*   PROCESSOR   : S3c2410x (32 bit ARM920T RISC core from Samsung)        *
*                                                                         *
*   TOOL-CHAIN  : SDT 2.51, ADS 1.2 or GCC                                *
*                                                                         *
*   DESCRIPTION :                                                         *
*   External interface & Module configuration header for MMU module.      *
*                                                                         *
**************************************************************************/


#ifndef __MMU_H__
#define __MMU_H__


#include "mmulib.h"


#ifdef __cplusplus
extern "C" {
#endif


/* ********************************************************************* */
/* Module configuration */


/* ********************************************************************* */
/* Interface macro & data definition */
#define MMUTT_SADDR	0x33ff8000
#define DESC_SEC       (0x2|(1<<4))
#define CB             (3<<2)  //cache_on, write_back
#define CNB            (2<<2)  //cache_on, write_through 
#define NCB            (1<<2)  //cache_off,WR_BUF on
#define NCNB           (0<<2)  //cache_off,WR_BUF off
#define AP_RW          (3<<10) //supervisor=RW, user=RW
#define AP_RO          (2<<10) //supervisor=RW, user=RO

#define DOMAIN_FAULT   (0x0)
#define DOMAIN_CHK     (0x1) 
#define DOMAIN_NOTCHK  (0x3) 
#define DOMAIN0        (0x0<<5)
#define DOMAIN1        (0x1<<5)

#define DOMAIN0_ATTR   (DOMAIN_CHK<<0) 
#define DOMAIN1_ATTR   (DOMAIN_FAULT<<2) 

#define RW_CB          (AP_RW|DOMAIN0|CB|DESC_SEC)
#define RW_CNB         (AP_RW|DOMAIN0|CNB|DESC_SEC)
#define RW_NCNB        (AP_RW|DOMAIN0|NCNB|DESC_SEC)
#define RW_FAULT       (AP_RW|DOMAIN1|NCNB|DESC_SEC)



/* ********************************************************************* */
/* Interface function definition */

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
void MMU_Init(void);

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
void MMU_SetMTT(unsigned int wVSAddr,unsigned int wVEAddr,unsigned int wPSAddr,unsigned int wAttrib);


/* ********************************************************************* */

#ifdef __cplusplus
}
#endif

#endif /*__MMU_H__*/
