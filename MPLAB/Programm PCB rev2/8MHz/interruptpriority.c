#include "interruptpriority.h"

#include "main.h"

//��� ��������� ����������� SetInterruptPriority() ������������ � ����� interruptpriority.h !!!

//_____________________________________________________________________________________
void SetInterruptPriority(void){//������� ��������� ����������� ����������
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //��� ���������  
    INTCON1bits.NSTDIS = NSTDIS_VAL;    //������������/�� ������������ ���������� ����������� � ��������� (������ interruptpriority.h)
    SRbits.IPL = CPUPRIORITY_VAL;	//���������� ���������� ���������
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //��������� ����������� ���������� 
    //PIC24FJ256GA110 Family Data Sheet (TABLE 7-2: IMPLEMENTED INTERRUPT VECTORS)
    IPC2bits.U1RXIP     = URXIP_VAL;        //��������� ���������� �� ������ UART1
    IPC3bits.U1TXIP     = UTXIP_VAL;        //��������� ���������� �� �������� UART1
    //IPC16bits.U1ERIP = UERIP_U2;      //UART2 error priority set higher then U1RXIP and U1TXIP
    IPC4bits.CNIP       = CNIP_VAL;         //��������� ����������� �� ������� CN
    IPC0bits.T1IP       = T1IP_VAL;         //��������� ���������� �� Timer1
    IPC1bits.T2IP       = T2IP_VAL;         //��������� ���������� �� Timer2
    IPC2bits.T3IP       = T3IP_VAL;         //��������� ���������� �� Timer3
    IPC6bits.T4IP       = T4IP_VAL;         //��������� ���������� �� Timer4
    IPC3bits.AD1IP      = ADCIP_VAL;         //��������� ���������� �� ���
}//END SetInterruptPriority()

