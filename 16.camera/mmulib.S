@ **************************************************************************
@                                                                          *
@    PROJECT     : ARM port for UCOS-II                                    *
@                                                                          *
@    MODULE      : MMULIB.s                                                *
@                                                                          *
@    AUTHOR      : Michael Anburaj                                         *
@                  URL  : http://geocities.com/michaelanburaj/             *
@                  EMAIL: michaelanburaj@hotmail.com                       *
@                                                                          *
@    PROCESSOR   : S3c2410x (32 bit ARM920T RISC core from Samsung)        *
@                                                                          *
@    IDE         : SDT 2.51 & ADS 1.2                                      *
@                                                                          *
@    DESCRIPTION :                                                         *
@    S3c2410x processor mmu assembly library file.                         *
@                                                                          *
@ *************************************************************************/




@ ==============
@  CPSR I,F bit
@ ==============
@ int SET_IF(void)@ 
@ The return value is current CPSR.
.globl    SET_IF
SET_IF:
     @ This function works only if the processor is in previliged mode.
    mrs r0,cpsr
    mov r1,r0
    orr r1,r1,#0xc0
    msr cpsr_cxsf,r1        
    mov pc,lr

@ void WR_IF(int cpsrValue)@ 
.globl WR_IF
WR_IF:
    @ This function works only if the processor is in previliged mode.
    msr cpsr_cxsf,r0        
    mov pc,lr


@ void CLR_IF(void)@ 
.globl  CLR_IF
CLR_IF:
     @ This function works only if the processor is in previliged mode.
    mrs r0,cpsr
    bic r0,r0,#0xc0
    msr cpsr_cxsf,r0        
    mov pc,lr

@ ====================================
@  MMU Cache/TLB/etc on/off functions
@ ====================================


@ void MMU_EnableICache(void)
.globl MMU_EnableICache
MMU_EnableICache:
    mrc p15,0,r0,c1,c0,0
    orr r0,r0,#(1<<12)
    mcr p15,0,r0,c1,c0,0
    mov pc,lr

@ void MMU_DisableICache(void)
.globl MMU_DisableICache
MMU_DisableICache:
    mrc p15,0,r0,c1,c0,0
    bic r0,r0,#(1<<12)
    mcr p15,0,r0,c1,c0,0
    mov pc,lr

@ void MMU_EnableDCache(void)
.globl MMU_EnableDCache
MMU_EnableDCache:
    mrc p15,0,r0,c1,c0,0
    orr r0,r0,#(1<<2)
    mcr p15,0,r0,c1,c0,0
    mov pc,lr

@ void MMU_DisableDCache(void)
.globl MMU_DisableDCache
MMU_DisableDCache:
    mrc p15,0,r0,c1,c0,0
    bic r0,r0,#(1<<2)
    mcr p15,0,r0,c1,c0,0
    mov pc,lr

@ void MMU_EnableAlignFault(void)
.globl MMU_EnableAlignFault 
MMU_EnableAlignFault:
    mrc p15,0,r0,c1,c0,0
    orr r0,r0,#(1<<1)
    mcr p15,0,r0,c1,c0,0
    mov pc,lr

@ void MMU_DisableAlignFault(void)
.globl MMU_DisableAlignFault
MMU_DisableAlignFault:
    mrc p15,0,r0,c1,c0,0
    bic r0,r0,#(1<<1)
    mcr p15,0,r0,c1,c0,0
    mov pc,lr

@ void MMU_EnableMMU(void)
.globl MMU_EnableMMU
MMU_EnableMMU:
    mrc p15,0,r0,c1,c0,0
    orr r0,r0,#(1)
    mcr p15,0,r0,c1,c0,0
    mov pc,lr

@ void MMU_DisableMMU(void)
.globl MMU_DisableMMU
MMU_DisableMMU:
    mrc p15,0,r0,c1,c0,0
    bic r0,r0,#(1)
    mcr p15,0,r0,c1,c0,0
    mov pc,lr

@ void MMU_SetFastBusMode(void)
@  FCLK:HCLK= 1:1
.globl MMU_SetFastBusMode
MMU_SetFastBusMode:
    mrc p15,0,r0,c1,c0,0
    bic r0,r0,#((1<<31)|(1<<30))
    mcr p15,0,r0,c1,c0,0
    mov pc,lr

@ void MMU_SetAsyncBusMode(void)
@  FCLK:HCLK= 1:2
.globl MMU_SetAsyncBusMode
MMU_SetAsyncBusMode:
    mrc p15,0,r0,c1,c0,0
    orr r0,r0,#((1<<30)|(1<<31))
    mcr p15,0,r0,c1,c0,0
    mov pc,lr

@ =========================
@  Set TTBase
@ =========================
@ void MMU_SetTTBase(int base)
.globl MMU_SetTTBase
MMU_SetTTBase:
    @ ro=TTBase
    mcr p15,0,r0,c2,c0,0
    mov pc,lr

@ =========================
@  Set Domain
@ =========================
@ void MMU_SetDomain(int domain)
.globl MMU_SetDomain
MMU_SetDomain:
    @ ro=domain
    mcr p15,0,r0,c3,c0,0
    mov pc,lr

@ =========================
@  ICache/DCache functions
@ =========================
@ void MMU_InvalidateIDCache(void)
.globl MMU_InvalidateIDCache
MMU_InvalidateIDCache:
    mcr p15,0,r0,c7,c7,0
    mov pc,lr

@ void MMU_InvalidateICache(void)
.globl MMU_InvalidateICache
MMU_InvalidateICache:
    mcr p15,0,r0,c7,c5,0
    mov pc,lr

@ void MMU_InvalidateICacheMVA(U32 mva)
.globl MMU_InvalidateICacheMVA
MMU_InvalidateICacheMVA:
    @ r0=mva
    mcr p15,0,r0,c7,c5,1
    mov pc,lr
    
@ void MMU_PrefetchICacheMVA(U32 mva)
.globl MMU_PrefetchICacheMVA
MMU_PrefetchICacheMVA:
    @ r0=mva
    mcr p15,0,r0,c7,c13,1
    mov pc,lr

@ void MMU_InvalidateDCache(void)
.globl MMU_InvalidateDCache
MMU_InvalidateDCache:
    mcr p15,0,r0,c7,c6,0
    mov pc,lr

@ void MMU_InvalidateDCacheMVA(U32 mva)
.globl MMU_InvalidateDCacheMVA
MMU_InvalidateDCacheMVA:
    @ r0=mva
    mcr p15,0,r0,c7,c6,1
    mov pc,lr

@ void MMU_CleanDCacheMVA(U32 mva)
.globl MMU_CleanDCacheMVA
MMU_CleanDCacheMVA:
    @ r0=mva
    mcr p15,0,r0,c7,c10,1
    mov pc,lr

@ void MMU_CleanInvalidateDCacheMVA(U32 mva)
.globl MMU_CleanInvalidateDCacheMVA
MMU_CleanInvalidateDCacheMVA:
    @ r0=mva
    mcr p15,0,r0,c7,c14,1
    mov pc,lr

@ void MMU_CleanDCacheIndex(U32 index)
.globl MMU_CleanDCacheIndex
MMU_CleanDCacheIndex:
    @ r0=index 
    mcr p15,0,r0,c7,c10,2
    mov pc,lr

@ void MMU_CleanInvalidateDCacheIndex(U32 index)    
.globl MMU_CleanInvalidateDCacheIndex
MMU_CleanInvalidateDCacheIndex:
    @ r0=index
    mcr p15,0,r0,c7,c14,2
    mov pc,lr

@ void MMU_WaitForInterrupt(void)
.globl MMU_WaitForInterrupt 
MMU_WaitForInterrupt:
    mcr p15,0,r0,c7,c0,4
    mov pc,lr

@ ===============
@  TLB functions
@ ===============
@ voic MMU_InvalidateTLB(void)
.globl MMU_InvalidateTLB
MMU_InvalidateTLB:
    mcr p15,0,r0,c8,c7,0
    mov pc,lr

@ void MMU_InvalidateITLB(void)
.globl MMU_InvalidateITLB
MMU_InvalidateITLB:
    mcr p15,0,r0,c8,c5,0
    mov pc,lr

@ void MMU_InvalidateITLBMVA(U32 mva)
.globl MMU_InvalidateITLBMVA
MMU_InvalidateITLBMVA:
    @ ro=mva
    mcr p15,0,r0,c8,c5,1
    mov pc,lr

@ void MMU_InvalidateDTLB(void)
.globl MMU_InvalidateDTLB
MMU_InvalidateDTLB:
    mcr p15,0,r0,c8,c6,0
    mov pc,lr

@ void MMU_InvalidateDTLBMVA(U32 mva)
.globl MMU_InvalidateDTLBMVA 
MMU_InvalidateDTLBMVA:
    @ r0=mva
    mcr p15,0,r0,c8,c6,1
    mov pc,lr

@ =================
@  Cache lock down
@ =================
@ void MMU_SetDCacheLockdownBase(U32 base)
.globl MMU_SetDCacheLockdownBase 
MMU_SetDCacheLockdownBase:
    @ r0= victim & lockdown base
    mcr p15,0,r0,c9,c0,0
    mov pc,lr
    
@ void MMU_SetICacheLockdownBase(U32 base)
.globl MMU_SetICacheLockdownBase
MMU_SetICacheLockdownBase:
    @ r0= victim & lockdown base
    mcr p15,0,r0,c9,c0,1
    mov pc,lr

@ =================
@  TLB lock down
@ =================
@ void MMU_SetDTLBLockdown(U32 baseVictim)
.globl MMU_SetDTLBLockdown
MMU_SetDTLBLockdown:
    @ r0= baseVictim
    mcr p15,0,r0,c10,c0,0
    mov pc,lr
    
@ void MMU_SetITLBLockdown(U32 baseVictim)
.globl MMU_SetITLBLockdown
MMU_SetITLBLockdown:
    @ r0= baseVictim
    mcr p15,0,r0,c10,c0,1
    mov pc,lr

@ ============
@  Process ID
@ ============
@ void MMU_SetProcessId(U32 pid)
.globl MMU_SetProcessId
MMU_SetProcessId:
    @ r0= pid
    mcr p15,0,r0,c13,c0,0
    mov pc,lr



