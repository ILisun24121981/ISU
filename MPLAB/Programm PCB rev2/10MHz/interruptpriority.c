#include "interruptpriority.h"

#include "main.h"

//Все настройки приоритетов SetInterruptPriority() выставляются в файле interruptpriority.h !!!

//_____________________________________________________________________________________
void SetInterruptPriority(void){//функция настройки приоритетов прерываний
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //Доп настройки  
    INTCON1bits.NSTDIS = NSTDIS_VAL;    //Использовать/не использовать приоритеты назначенные в программе (смотри interruptpriority.h)
    SRbits.IPL = CPUPRIORITY_VAL;	//Назначение приоритета программе
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //Настройка приоритетов прерываний 
    //PIC24FJ256GA110 Family Data Sheet (TABLE 7-2: IMPLEMENTED INTERRUPT VECTORS)
    IPC2bits.U1RXIP     = URXIP_VAL;        //Приоритет прерывания по приему UART1
    IPC3bits.U1TXIP     = UTXIP_VAL;        //Приоритет прерывания по передаче UART1
    //IPC16bits.U1ERIP = UERIP_U2;      //UART2 error priority set higher then U1RXIP and U1TXIP
    IPC4bits.CNIP       = CNIP_VAL;         //Приоритет прерыванией по выводам CN
    IPC0bits.T1IP       = T1IP_VAL;         //Приоритет прерывания по Timer1
    IPC1bits.T2IP       = T2IP_VAL;         //Приоритет прерывания по Timer2
    IPC2bits.T3IP       = T3IP_VAL;         //Приоритет прерывания по Timer3
    IPC6bits.T4IP       = T4IP_VAL;         //Приоритет прерывания по Timer4
    IPC3bits.AD1IP      = ADCIP_VAL;         //Приоритет прерывания по АЦП
}//END SetInterruptPriority()

