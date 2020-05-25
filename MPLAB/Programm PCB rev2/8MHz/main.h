#ifndef _MAIN_H        
#define _MAIN_H 

#include <p24Fxxxx.h>
#include <p24fj128ga106.h>
#include <libpic30.h>

#include "Command.h"

#if !defined(__PACKED)
        #define __PACKED
#endif

#define FW_Version 2

#define	FCY	16000000  //�������� �������
//_____________________________________________________________________________________
//  ���������� "����������" �������� 

#define ON              1
#define OFF             0

#define DET             1
#define NOT_DET		0

#define YES             1
#define NO		0

#define ENABLE_STATE    1
#define DISABLE_STATE   0

#define START           1 //������ ��������
#define STOP            0 //����� ��������, ������� �� �������, ������� ��������
#define WAIT            2 //��������

#define DEFAULT         0

#define ADJUST          1
#define STANDART        2

#define ADJUST1         1
#define STANDART1       2

#define NORMAL   	1
#define NORMAL_SM1150   0
#define ALARM     	0

#define ALARM_LOW	2
//#define LOW   		2
//#define HIGH	 	3
#define ALARM_HIGH 	0
#define ERROR           6

//��������� ������� ������������� �������(� ������ ������������� �������).������������ � �������� ������� �� ������� ������������� ���������� Reason
#define SM1150_ALARM                        1
#define GAS_PRESSURE_SENSOR_ALARM           2
#define GAS_PLASMA_FLOW_SENSOR_ALARM        3
#define GAS_TRANSPORT_FLOW_SENSOR_ALARM     4
#define GAS_EXTRACT_SENSOR_ALARM            5
#define PMT_SENSOR_OFF                      6

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ���������� ����������
extern unsigned char Reason;//���������� ������������� ������� ������� ������� � ���������� �������� ������� �� ������������ ���������� (������� �� ������� ������� ����������� ������)
//_____________________________________________________________________________________
//  ���������� �������
void InitPic(void);
void AnalyseRxPacket(void);//������� ������� ������������� ��������� ������
void ReasonCommand(void);//
void CheckModuleState(void);
void __attribute__((interrupt, no_auto_psv)) _AddressError (void);
void __attribute__((interrupt, no_auto_psv)) _StackError (void);
void __attribute__((interrupt, no_auto_psv)) _MathError (void);
void __attribute__((interrupt, no_auto_psv)) _OscillatorFail (void);

#endif //END #ifndef _MAIN_H
//_____________________________________________________________________________________
//HELP ������������

//1. �����������  � ������ �������� ����� ��� ������ ����� (# _TRISD3  - ����������� # U2MODEbits.PDSEL0, U2MODEbits.PDSEL  - ������)
/*


��� ������������� ������ �/��� ����������� ���� �� pic24fj128GA106.h - ���� ��������� � ����� ������������ �30 ������ Support.
������� � ���� ����� �������� �������� ���� # ��� U1MODE

	__extension__ typedef struct tagU2MODEBITS {
  union {
    struct {
      unsigned STSEL:1;
      unsigned PDSEL0:1;
      unsigned PDSEL1:1;
      unsigned BRGH:1;
      unsigned RXINV:1;
      unsigned ABAUD:1;
      unsigned LPBACK:1;
      unsigned WAKE:1;
      unsigned UEN0:1;
      unsigned UEN1:1;
      unsigned :1;
      unsigned RTSMD:1;
      unsigned IREN:1;
      unsigned USIDL:1;
      unsigned :1;
      unsigned UARTEN:1;
    };
    struct {
      unsigned :1;
      unsigned PDSEL:2;
      unsigned :5;
      unsigned UEN:2;
    };
  };
} U2MODEBITS;

�������������� ������ �� �����, ��� ����� ������������
������ ���:
		��� ���� PDSEL0  - U2MODEbits.PDSEL0
		��� ������ ����� PDSEL0 � PDSEL1- U2MODEbits.PDSEL


��� ������������� ����������� ����
������� � ���� ����� �������� ������ ������� # ��� TRISD

	/ TRISD /
#define _TRISD0 TRISDbits.TRISD0
#define _TRISD1 TRISDbits.TRISD1
#define _TRISD2 TRISDbits.TRISD2
#define _TRISD3 TRISDbits.TRISD3
#define _TRISD4 TRISDbits.TRISD4
#define _TRISD5 TRISDbits.TRISD5
#define _TRISD6 TRISDbits.TRISD6
#define _TRISD7 TRISDbits.TRISD7
#define _TRISD8 TRISDbits.TRISD8
#define _TRISD9 TRISDbits.TRISD9
#define _TRISD10 TRISDbits.TRISD10
#define _TRISD11 TRISDbits.TRISD11

�������������� ������ �� �����, ��� ����� ������������
����������� ���:
		��� ���� TRISD0(������ ��� TRISDbits.TRISD0) - _TRISD0


��������� ������ �������� ����������� ���� # ��� U1MODE

/ U1MODE /
/ Bitname _STSEL cannot be defined because it is used by more than one SFR /
/ Bitname _PDSEL0 cannot be defined because it is used by more than one SFR /
/ Bitname _PDSEL1 cannot be defined because it is used by more than one SFR /
/ Bitname _BRGH cannot be defined because it is used by more than one SFR /
/ Bitname _RXINV cannot be defined because it is used by more than one SFR /
/ Bitname _ABAUD cannot be defined because it is used by more than one SFR /
/ Bitname _LPBACK cannot be defined because it is used by more than one SFR /
/ Bitname _WAKE cannot be defined because it is used by more than one SFR /
/ Bitname _UEN0 cannot be defined because it is used by more than one SFR /
/ Bitname _UEN1 cannot be defined because it is used by more than one SFR /
/ Bitname _RTSMD cannot be defined because it is used by more than one SFR /
/ Bitname _IREN cannot be defined because it is used by more than one SFR /
/ Bitname _USIDL cannot be defined because it is used by more than one SFR /
/ Bitname _UARTEN cannot be defined because it is used by more than one SFR /
/ Bitname _PDSEL cannot be defined because it is used by more than one SFR /
/ Bitname _UEN cannot be defined because it is used by more than one SFR /

E��� ��������� ��� ������ ��� (������ �����) �� ����� ����� ����������� �������� ��� ��� ������������ ���������� �������� ����
� ������ ��������� SFR,  �� ��� ����������� �������� - ���������� ������.


� ������ ��������� ������������ ��� ����������� ��� � ������ ����� (���� ���� ���� ����������� ��� ������� �������)
*/
//2. ��������� ������������� ������ ������� ����������� (# UART)
/*
����� ��������� �������������:
	1.������ �������� (����� ������ ������ DISABLE)
	2.������������ ������ ������� ���������� ���������/���������, ����� ���������� �������.
*/
//3 ��������� ����������� ����������
/*
	��� �������� ��������� ����������� ���������� ������������ ��������� ������� InterruptPriority
	���� ��������� ������������� ���������� ��� ������ ��������� ���������� ������� ���� ����������� ISR
*/
//4. ��������� ������������� ����������� ������� (# StepMotor )
/*
	����� ��������� �������������:
	1.������ �������� (����� ������ ������ DISABLE)
	2.���������� ��������� �� ���������(Default Presets)*/


