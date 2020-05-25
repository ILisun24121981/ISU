#ifndef _TIMER_H        
#define _TIMER_H
//_____________________________________________________________________________________
// ����������� ��������
    #define MAIN_PACKET_DALAY  200 //�������� �������� ��������� ������(ms - Timer1Interrupt)
//_____________________________________________________________________________________
// ���������� ����������
    extern volatile unsigned int MainPacketDelay;
//_____________________________________________________________________________________
// ���������� �������
    void InitTimers(void);
    void EnableTimer1(void);
    void EnableTimer2(void);
    void EnableTimer3(void);
    void EnableTimer4(void);
//--------------------------------------------------------------
    void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void);	//(P=4 T=1ms )
    void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void); 	//(P=6 T=25us)
    void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void);	//(P=1 ������� �������� � ������� ��������� ���)
    //void __attribute__((interrupt, no_auto_psv)) _T4Interrupt(void);	//(P=3 T=10ms)
    //void __attribute__((interrupt, no_auto_psv)) _T5Interrupt(void);
//_____________________________________________________________________________________
// ����������� ��������
    #define ClearFlagTimer1Int()    IFS0bits.T1IF   =0;     //�������� ���� ���������� �� Timer1
    #define SetFlagTimer1Int()      IFS0bits.T1IF   =1;     //���������� ���� ���������� �� Timer1
    #define EnTimer1Int()           IEC0bits.T1IE   =1;     //�������� ���������� �� Timer1
    #define DisTimer1Int()          IEC0bits.T1IE   =0;     //��������� ���������� �� Timer1
    #define OnTimer1()              T1CONbits.TON   =1;     //�������� Timer1
    #define OffTimer1()             T1CONbits.TON   =0;     //��������� Timer1

    #define ClearFlagTimer2Int()    IFS0bits.T2IF   =0;     //�������� ���� ���������� �� Timer2
    #define SetFlagTimer2Int()      IFS0bits.T2IF   =1;     //���������� ���� ���������� �� Timer2
    #define EnTimer2Int()           IEC0bits.T2IE   =1;     //�������� ���������� �� Timer2
    #define DisTimer2Int()          IEC0bits.T2IE   =0;     //��������� ���������� �� Timer2
    #define OnTimer2()              T2CONbits.TON   =1;     //�������� Timer2
    #define OffTimer2()             T2CONbits.TON   =0;     //��������� Timer2

    #define ClearFlagTimer3Int()    IFS0bits.T3IF   =0;     //�������� ���� ���������� �� Timer3
    #define SetFlagTimer3Int()      IFS0bits.T3IF   =1;     //���������� ���� ���������� �� Timer3
    #define EnTimer3Int()           IEC0bits.T3IE   =1;     //�������� ���������� �� Timer3
    #define DisTimer3Int()          IEC0bits.T3IE   =0;     //��������� ���������� �� Timer3
    #define OnTimer3()              T3CONbits.TON   =1;     //�������� Timer3
    #define OffTimer3()             T3CONbits.TON   =0;     //��������� Timer3

    #define ClearFlagTimer4Int()    IFS1bits.T4IF   =0;     //�������� ���� ���������� �� Timer4
    #define SetFlagTimer4Int()      IFS1bits.T4IF   =1;     //���������� ���� ���������� �� Timer4
    #define EnTimer4Int()           IEC1bits.T4IE   =1;     //�������� ���������� �� Timer4
    #define DisTimer4Int()          IEC1bits.T4IE   =0;     //��������� ���������� �� Timer4
    #define OnTimer4()              T4CONbits.TON   =1;     //�������� Timer4
    #define OffTimer4()             T4CONbits.TON   =0;     //��������� Timer4

#endif

