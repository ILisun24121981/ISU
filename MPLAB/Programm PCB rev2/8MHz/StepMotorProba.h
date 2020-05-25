#ifndef _STEPMOTOR_PROBA_H
#define _STEPMOTOR_PROBA_H

#include "main.h"//��� ����������� ������������� "����������" �������� (# ON,OFF, STOP)
#include "timer.h"
//_____________________________________________________________________________________
// ����������� ��������
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Default Presets
//#define DEFAULT_MAX_SPEED			3906//������������ ��������� �������� ��� ������� ��
//#define	DEFAULT_CONTROL_VOLUME                  300//����������� ����� �������� �� ��������� ����� ����������� ���������� ������ ����������� ��� ������� ��

#define	DEFAULT_MIN_SPEED                       0x7A12//2���� � ��� - ��� ������� ���� Speed - ����������� �� ���� ������������ ��������  - ������������ �������� �� ����� ���� ���� 2 �/���
#define	DEFAULT_TRANSFERT_COEFF_SL		0x1
#define	DEFAULT_TRANSFERT_COEFF_LV              0x1
#define	DEFAULT_MAX_MAX_CONTROL_VOLUME		65000//����������� �� ���� ������������� ������������ ������.


//#define	DEFAULT_THIRD_PHASE_END_POINT		200
//#define	DEFAULT_FOURTH_PHASE_END_POINT		250
//
//#define DEFAULT_ZERO_PHASE_SPEED		3906//�������� �������� PR2 (��. timer.c) ��� ������ �������� ��� ������ ��������
//#define	DEFAULT_FIRST_PHASE_SPEED               7812
//#define	DEFAULT_SECOND_PHASE_SPEED		15624
//#define	DEFAULT_THIRD_PHASE_SPEED               31248
//#define	DEFAULT_FOURTH_PHASE_SPEED		7812
//#define DEFAULT_WASHING_SPEED			7812
//#define DEFAULT_RETURN_SPEED			3906//������ ���� ������ DEFAULT_MAX_SPEED
//#define DEFAULT_MOVING_SPEED			3906//������ ���� ������ DEFAULT_MAX_SPEED


//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ���������� ������ ���������� ��������� ��.
//������� A
#define ENABLE_TRIS     _TRISE1
#define ENABLE_LAT      _LATE1
#define ENABLE_PORT     _RE1
#define ENABLE_OD       _ODE1
#define ENABLE_CNIE     _CN59IE
#define ENABLE_CNPUE	_CN59PUE
#define ENABLE_CNPDE	_CN59PDE
//������� �
#define RESET_TRIS	_TRISE0
#define RESET_LAT	_LATE0
#define RESET_PORT	_RE0
#define RESET_OD	_ODE0
#define RESET_CNIE	_CN58IE
#define RESET_CNPUE	_CN58PUE
#define RESET_CNPDE	_CN58PDE
//������� �
#define STEP_TRIS	_TRISF1
#define STEP_LAT	_LATF1
#define STEP_PORT	_RF1
#define STEP_OD     _ODF1
#define STEP_CNIE	_CN69IE
#define STEP_CNPUE	_CN69PUE
#define STEP_CNPDE	_CN69PDE
//������� �
#define DIR_TRIS	_TRISF0
#define DIR_LAT     _LATF0
#define DIR_PORT	_RF0
#define DIR_OD      _ODF0
#define DIR_CNIE	_CN68IE
#define DIR_CNPUE	_CN68PUE
#define DIR_CNPDE	_CN68PDE
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ����������  ������ ���������� ��
// �������� ������(��������)
#define END_TRAILER_TRIS	_TRISD1
#define END_TRAILER_LAT         _LATD1
#define END_TRAILER_PORT        _RD1
#define END_TRAILER_OD		_ODD1
#define END_TRAILER_CNIE        _CN50IE
#define END_TRAILER_CNPUE       _CN50PUE
#define END_TRAILER_CNPDE       _CN50PDE
// ��������� ������(��������)
#define START_TRAILER_TRIS	_TRISC14
#define START_TRAILER_LAT	_LATC14
#define START_TRAILER_PORT	_RC14
#define START_TRAILER_OD	_ODC14
#define START_TRAILER_CNIE	_CN0IE
#define START_TRAILER_CNPUE	_CN0PUE
#define START_TRAILER_CNPDE	_CN0PDE
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ��������� ��� ����������� ���� ��
#define ZERO		0
#define FIRST		1
#define SECOND		2
#define THIRD		3
#define FOURTH		4

//#define MOVING		5
//#define RETURN		6
//#define WASHING		7
 
#define PHASES_MOVING       1	//�������� �� � ������������ � ���������� �������(��������� ��������� �� �������)
#define WASHING_MOVING      2   //�������� �� � ������������ � ���������� ��������(�������� ���������� ���� ���)
#define SIMPLE_MOVING       3	//�������� �� � ���������� ���������

#define FORWARD		1
#define BACK		2

#define UP_TRAILER_FAIL_ON_STATE     2
#define UP_TRAILER_FAIL_OFF_STATE    3
#define DOWN_TRAILER_FAIL_ON_STATE   4
//#define DOWN_TRAILER_FAIL_OFF_STATE  5

#define CHECK_STEPS 200//���������� ����� ����������� ��� ���������������� ������������ ����������

//_____________________________________________________________________________________
// ���������� ����������

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ����������� �����

typedef struct {
    
    struct {//����������� ��� ������� ����������� � ������� SetDevSettings()
        unsigned short int MaxSpeed;
        unsigned short int TransferCoefficientSL;//���/��
        unsigned short int MaxControlVolume;
    } DeveloperSettings;//��������� ������������� �� ����� ��������� ������������

    struct {//����������� ��� ������� ����������� � ������� SetDevSettings()
        unsigned short int TransferCoefficientLV;//��/��
        unsigned short int ControlVolume;
        unsigned short int MaxSpeedOil;
        unsigned short int EndPhasePoint[5];
        unsigned short int PhaseSpeed[5];
    } DefaultPresets;//��������� ������������� �� ����� ��������� ������������

    struct {//����������� ��� ������������� � ������� InitStepMotor().
        unsigned short int TransferCoefficientLV;
        unsigned short int ControlVolume;
        unsigned short int MaxSpeedOil;
        unsigned short int EndPhasePoint[5];
        unsigned short int PhaseSpeed[5];
    } Presets; //��������� �������� �������� //���������� - CommandStepMotor()

    struct {//����������� ��� ������������� � ������� InitStepMotor().
        volatile unsigned char MovingState;//��������� ��������                               //���������� - CommandStepMotor(),SmTrailerReaction(void)
        volatile unsigned char Direction;//����������� �������� ��������                      //���������� - CommandStepMotor(),SmTrailerReaction(void),StepReaction(void)
        volatile unsigned short int Speed;//�������� �������� ��������                        //���������� - CommandStepMotor(),SmTrailerReaction(void),StepReaction(void)
        volatile signed long TargetPosition;//�������� ����� �������� ��������                //���������� - CommandStepMotor(),SmTrailerReaction(void),StepReaction(void)
    } MovingSettings; //��������� �������� �������� �������� - ���������� ��������� �������� �������� ��.//���������� CommandStepMotor(),SmTrailerReaction(void),StepReaction(),

    struct {
        volatile unsigned char WorkMode;//����� ������ �� //���������� - CommandStepMotor(),SmTrailerReaction()
        unsigned char Zero;
        unsigned char Block;//
        volatile unsigned char NeedStepReaction;//���� �������� ������� �� ����������� ���       //���������� - _T2Interrupt(void),StepReaction(void)
        volatile signed long GeneralStepCounter;//������� ����� �� ������� �������               //���������� - CommandStepMotor(),SmTrailerReaction(),_T2Interrupt(void)
        volatile unsigned short int MovingStepCounter;//������� ����� �������� ��������                //���������� - CommandStepMotor(),SmTrailerReaction(),_T2Interrupt(void)      
    } State;//��������� ��������� �� - ���������� ������� ��������� ��.//���������� - CommandStepMotor(), _T2Interrupt(void),SmTrailerReaction()

    struct {
        volatile unsigned char Down;
        volatile unsigned char DownBlock;
        volatile unsigned char Up;
        volatile unsigned char UpBlock;
        unsigned char Alarm;
    }Trailer;

} SM_t;
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ���������� ����������
extern volatile SM_t StepMotor;

//_____________________________________________________________________________________
// ���������� �������

void ConfigPortsStepMotor(void);
void InitStepMotor(void);
void CommandStepMotor(void);
void SmTrailerReaction(void);
void StepReaction(void);
void CheckStepMotorState(void);
void AlarmReactionStepMotor(void);
//_____________________________________________________________________________________
// ����������� ��������

#define StepMotor_On()	OnTimer2();\
                        SetFlagTimer2Int();
                        
#define StepMotor_Off() StepMotor.MovingSettings.Direction    = STOP;\
                        StepMotor.MovingSettings.Speed        = STOP;\
                        StepMotor.MovingSettings.TargetPosition = STOP;
                                        
#endif//END #ifndef _STEPMOTOR_H

