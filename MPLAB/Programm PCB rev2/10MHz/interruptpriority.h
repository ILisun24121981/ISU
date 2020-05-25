#ifndef _INTERRUPTPRIORITY_H        
#define _INTERRUPTPRIORITY_H 


//_____________________________________________________________________________________
// ����������� ��������
    //Note
    /*
     * An interrupt source programmed to priority level 0 is effectively disabled,
     * since it can never be greater than the CPU priority.
     * The usable priority levels start at level 1 as the lowest priority and level 7 as the highest priority.
    */
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // ��������� ��� ��������� (������ !=0 ���� ����� ������������ ��������������� ������� �����������)
    #define NSTDIS_VAL      0   //������������ ���������� ����������� � ���������
                                //1 - ��������� ����������
                                //� ������ ������������� ���������� ������������
                                //������������ ���������� �������� ������������ � ������� �������� (������� ����� - ������� ���������)
    #define CPUPRIORITY_VAL 0   //���������� ���������� ���������
                                //���������� ���� ������� ���������� �� ����� ��������� ���������.
                                //All user interrupt sources can be disabled by setting IPL<2:0> = 111. -> CPUPRIORITY = 7
    //Value at POR: NSTDIS =0;CPUPRIORITY =0;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // ��������� ��� ������� ����������� ��������������� ����������
    #define	UTXIP_VAL   3	//��������� ���������� �� �������� UART1
    #define	URXIP_VAL   7   //��������� ���������� �� ������ UART1
    #define     CNIP_VAL    7   //��������� ����������� �� ������� CN
    #define     T1IP_VAL    4   //��������� ���������� �� Timer1 (1ms)
    #define     T2IP_VAL    5   //��������� ���������� �� Timer2 (������ �������� ��� ������������� � ������� InitPMSettings()) - ������ ������ �� ����� ������ ��
    #define     T3IP_VAL    6   //��������� ���������� �� Timer3 (10ms) - ������� ������ �� ����� ������ ��������
    #define     T4IP_VAL    6   //��������� ���������� �� Timer4 (ms)
    #define     ADCIP_VAL   2
//_____________________________________________________________________________________
// ���������� �������
    void SetInterruptPriority(void);
#endif //END #ifndef _INTERRUPTPRIORITY_H

