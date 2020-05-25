#ifndef _CN_H
#define _CN_H

#include "main.h"
//_____________________________________________________________________________________
//����������� ��������
    
    #define SETTING_ON            0x10   //��������� �������� �� ��������� OFF � ��������� ON
    #define SETTING_OFF           0x11   //��������� �������� �� ��������� ON � ��������� OFF

    #define SETTING_NORMAL        0x12   //��������� �������� � ��������� NORMAL
    #define SETTING_ALARM         0x13   //��������� �������� � ��������� ALARM

//    #define SETTING_ALARM_LOW     0x14  //��������� �������� � ��������� ALARM_LOW
//    #define SETTING_LOW           0x15  //��������� �������� � ��������� LOW
//    #define SETTING_HIGH          0x16  //��������� �������� � ��������� HIGH
//    #define SETTING_ALARM_HIGH    0x17  //��������� �������� � ��������� ALARM_HIGH

    #define CHANGE                0x18  //��������� ��������� - ������������ � SM1150
    #define SETTING_ERROR         0x19  //������� � ������������ ��������� - ��������������� � ������������� ��� ������������ ��������� �������(��)
//_____________________________________________________________________________________
//  ���������� �������
    void CnReaction(void);
    void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);

//_____________________________________________________________________________________
// ����������� ��������
    #define EnableCnInt()     _CNIE = 1; //�������� ���������� �� ������� CN
    #define DisableCnInt()    _CNIE = 0; //��������� ���������� �� ������� CN
    #define SetFlagCnInt()    _CNIF = 1; //����������� ���� ���������� �� ������� CN
    #define ClearFlagCnInt()  _CNIF = 0; //�������� ���� ���������� �� ������� CN
#endif
