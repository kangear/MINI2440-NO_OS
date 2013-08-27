#define rGPBDATA        0x56000014

.globl IsrIRQ

.globl IsrIRQ
IsrIRQ:
        @ Fix the return address

	sub lr,lr,#4
        stmfd sp!,{r0-r7,r12,lr}  

#define rINTOFFSET 0x4a000014	
	LDR     R0, =rINTOFFSET
        LDR     R0, [R0]

        LDR     R1, =HandleEINT0
        MOV     LR, PC                          @ Save LR befor jump to the C function we need return back
        LDR     PC, [R1, R0, LSL #2]              
    
	ldmfd sp!,{r0-r7,r12,pc}^





   
