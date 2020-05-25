#ifndef _STEPMOTOR_PMT_H
#define _STEPMOTOR_PMT_H

#include "main.h"//��� ����������� ������������� "����������" �������� (# ON,OFF, STOP)
#include "timer.h"
//_____________________________________________________________________________________
// ����������� ��������
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Default Presets

#define	DEFAULT_SM1_MOVING_SPEED                    6250//10 ���� � ��� - ��� ������� ���� Speed - ����������� �� ���� ������������ ��������  - ������������ �������� �� ����� ���� ���� 2 �/���
#define	DEFAULT_SM1_MAX_CONTROL_VOLUME              3000//����������� �� ���� ������������� ������������ ������.


//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ���������� ������ ���������� ��������� ��.
//������� A
#define WINDING_A1_TRIS         _TRISD6
#define WINDING_A1_LAT          _LATD6
#define WINDING_A1_PORT         _RD6
#define WINDING_A1_OD           _ODD6
#define WINDING_A1_CNIE         _CN15IE
#define WINDING_A1_CNPUE	_CN15PUE
#define WINDING_A1_CNPDE	_CN15PDE
//������� �
#define WINDING_B1_TRIS         _TRISD5
#define WINDING_B1_LAT          _LATD5
#define WINDING_B1_PORT         _RD5
#define WINDING_B1_OD           _ODD5
#define WINDING_B1_CNIE         _CN14IE
#define WINDING_B1_CNPUE	_CN14PUE
#define WINDING_B1_CNPDE	_CN14PDE
//������� �
#define WINDING_C1_TRIS         _TRISD4
#define WINDING_C1_LAT          _LATD4
#define WINDING_C1_PORT         _RD4
#define WINDING_C1_OD           _ODD4
#define WINDING_C1_CNIE         _CN13IE
#define WINDING_C1_CNPUE	_CN13PUE
#define WINDING_C1_CNPDE	_CN13PDE
//������� �
#define WINDING_D1_TRIS         _TRISD3
#define WINDING_D1_LAT          _LATD3
#define WINDING_D1_PORT         _RD3
#define WINDING_D1_OD           _ODD3
#define WINDING_D1_CNIE         _CN52IE
#define WINDING_D1_CNPUE	_CN52PUE
#define WINDING_D1_CNPDE	_CN52PDE
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ����������  ������ ���������� ��
// �������� ���������� ���������
#define ZERO_TRAILER_TRIS	_TRISD2
#define ZERO_TRAILER_LAT	_LATD2
#define ZERO_TRAILER_PORT	_RD2
#define ZERO_TRAILER_OD		_ODD2
#define ZERO_TRAILER_CNIE	_CN51IE
#define ZERO_TRAILER_CNPUE	_CN51PUE
#define ZERO_TRAILER_CNPDE	_CN51PDE

#define FORWARD1		1
#define BACK1                   2
#define MOVING1                 3
//_____________________________________________________________________________________
// ���������� ����������

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ����������� �����

typedef struct {

    struct {//����������� ��� ������������� � ������� InitStepMotor().
        unsigned char MovingState;
        unsigned char Direction;//����������� �������� ��������
        volatile signed short int TargetPosition;//�������� ����� �������� ��������
    } MovingSettings; //��������� �������� �������� �������� - ���������� ��������� �������� �������� ��.

    struct {
        unsigned char DeterminatePositionMarker; // ������ ��������� ��� ���� ����������� ��������� �� ��������� �������� ������ ����������� �������
        unsigned char ZeroSetMarker;// ������ ���������,��� ���� ������� ��������� � ���� ������ ������ ���������� ( �.�. ��� ��������� ������������ ��������� ��������� �� ����� �����������, ����� ������ ���������)
        unsigned char InSavedPositionMarker;//������ ���������,��� ������� ������� ������������� ������� ���������� � ������ ��������
        signed short int SavedPosition;
        signed short int GeneralStepCounter;//������� ����� �� ������� �������
        unsigned short int MovingStepCounter;//������� ����� �������� ��������

    } State;//��������� ��������� �� - ���������� ������� ��������� ��.

    struct {
        unsigned char State;
    }Trailer;

} SM1_t;
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ���������� ����������
extern volatile SM1_t StepMotor1;

//_____________________________________________________________________________________
// ���������� �������

void ConfigPortsStepMotor1(void);
void InitStepMotor1(void);
void CommandStepMotor1(void);
void SmTrailerReaction1(void);
void StepReaction1(void);
//_____________________________________________________________________________________
// ����������� ��������

#define StepMotor1_On()	OnTimer4();

#define StepMotor1_Off() StepMotor1.MovingSettings.Direction    = STOP;\
                        StepMotor1.MovingSettings.TargetPosition = STOP;

#endif//END #ifndef _STEPMOTOR_PMT_H



