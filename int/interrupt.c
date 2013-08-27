#include "s3c24xx.h"

void EINT_Handle()
{
    unsigned long oft = INTOFFSET;
    unsigned long val;
    
    switch( oft )
    {
        // K4被按下
        case 0: 
        {   
            GPBDAT |= (0x0f<<5);   // 所有LED熄灭
            GPBDAT &= ~(1<<8);      // LED4点亮
            break;
        }
        
        // K3被按下
        case 2:
        {   
            GPBDAT |= (0x0f<<5);   // 所有LED熄灭
            GPBDAT &= ~(1<<7);      // LED3点亮
            break;
        }

        // K1或K2被按下
        case 5:
        {   
            GPBDAT |= (0x0f<<5);   // 所有LED熄灭
            
            // 需要进一步判断是K1还是K2，或是K1、K2被同时按下
            val = EINTPEND;
            if (val & (1<<14))
                GPBDAT &= ~(1<<8);      // K4被按下，LED4点亮
            if (val & (1<<13))
                GPBDAT &= ~(1<<7);      // K3被按下，LED3点亮
            if (val & (1<<11))
                GPBDAT &= ~(1<<6);      // K2被按下，LED2点亮
            if (val & (1<<8))
                GPBDAT &= ~(1<<5);      // K1被按下，LED1点亮
                
            break;
        }

        default:
            break;
    }

    //清中断
    if( oft == 5 ) 
    EINTPEND = (1<<8) | (1<<11) | (1<<13) | (1<<14);   // EINT8_23合用IRQ5
    SRCPND = 1<<oft;
    INTPND = 1<<oft;
}
