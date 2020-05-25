#ifndef _STEPMOTOR_PROBA_H
#define _STEPMOTOR_PROBA_H

#include "main.h"//для возможности использования "глобальных" констант (# ON,OFF, STOP)
#include "timer.h"
//_____________________________________________________________________________________
// Определение констант
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Default Presets
//#define DEFAULT_MAX_SPEED			3906//максимальная возможная скорость для данного ШД
//#define	DEFAULT_CONTROL_VOLUME                  300//контрольный объем движения по умолчанию равен максимально возможному объему перемещения для данного ШД

#define	DEFAULT_MIN_SPEED                       0x7A12//2шага в сек - для расчета файл Speed - ограничение на ввод максимальной скорости  - максимальная скорость не может быть ниже 2 ш/сек
#define	DEFAULT_TRANSFERT_COEFF_SL		0x1
#define	DEFAULT_TRANSFERT_COEFF_LV              0x1
#define	DEFAULT_MAX_MAX_CONTROL_VOLUME		65000//ограничение на ввод максимального контрольного объема.


//#define	DEFAULT_THIRD_PHASE_END_POINT		200
//#define	DEFAULT_FOURTH_PHASE_END_POINT		250
//
//#define DEFAULT_ZERO_PHASE_SPEED		3906//Значения регистра PR2 (см. timer.c) чем больше значение тем меньше скорость
//#define	DEFAULT_FIRST_PHASE_SPEED               7812
//#define	DEFAULT_SECOND_PHASE_SPEED		15624
//#define	DEFAULT_THIRD_PHASE_SPEED               31248
//#define	DEFAULT_FOURTH_PHASE_SPEED		7812
//#define DEFAULT_WASHING_SPEED			7812
//#define DEFAULT_RETURN_SPEED			3906//должно быть больше DEFAULT_MAX_SPEED
//#define DEFAULT_MOVING_SPEED			3906//должно быть больше DEFAULT_MAX_SPEED


//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Назначение портов управления обмотками ШД.
//Обмотка A
#define ENABLE_TRIS     _TRISE1
#define ENABLE_LAT      _LATE1
#define ENABLE_PORT     _RE1
#define ENABLE_OD       _ODE1
#define ENABLE_CNIE     _CN59IE
#define ENABLE_CNPUE	_CN59PUE
#define ENABLE_CNPDE	_CN59PDE
//Обмотка Б
#define RESET_TRIS	_TRISE0
#define RESET_LAT	_LATE0
#define RESET_PORT	_RE0
#define RESET_OD	_ODE0
#define RESET_CNIE	_CN58IE
#define RESET_CNPUE	_CN58PUE
#define RESET_CNPDE	_CN58PDE
//Обмотка В
#define STEP_TRIS	_TRISF1
#define STEP_LAT	_LATF1
#define STEP_PORT	_RF1
#define STEP_OD     _ODF1
#define STEP_CNIE	_CN69IE
#define STEP_CNPUE	_CN69PUE
#define STEP_CNPDE	_CN69PDE
//Обмотка Г
#define DIR_TRIS	_TRISF0
#define DIR_LAT     _LATF0
#define DIR_PORT	_RF0
#define DIR_OD      _ODF0
#define DIR_CNIE	_CN68IE
#define DIR_CNPUE	_CN68PUE
#define DIR_CNPDE	_CN68PDE
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Назначение  портов концевиков ШД
// Конечный датчик(концевик)
#define END_TRAILER_TRIS	_TRISD1
#define END_TRAILER_LAT         _LATD1
#define END_TRAILER_PORT        _RD1
#define END_TRAILER_OD		_ODD1
#define END_TRAILER_CNIE        _CN50IE
#define END_TRAILER_CNPUE       _CN50PUE
#define END_TRAILER_CNPDE       _CN50PDE
// Начальный датчик(концевик)
#define START_TRAILER_TRIS	_TRISC14
#define START_TRAILER_LAT	_LATC14
#define START_TRAILER_PORT	_RC14
#define START_TRAILER_OD	_ODC14
#define START_TRAILER_CNIE	_CN0IE
#define START_TRAILER_CNPUE	_CN0PUE
#define START_TRAILER_CNPDE	_CN0PDE
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Константы для наглядности кода ШД
#define ZERO		0
#define FIRST		1
#define SECOND		2
#define THIRD		3
#define FOURTH		4

//#define MOVING		5
//#define RETURN		6
//#define WASHING		7
 
#define PHASES_MOVING       1	//движение ШД в соответствии с алгоритмом анализа(изменение скоростей по объемам)
#define WASHING_MOVING      2   //движение ШД в соответствии с алгоритмом промывки(скорость изменяется один раз)
#define SIMPLE_MOVING       3	//движение ШД с постоянной скоростью

#define FORWARD		1
#define BACK		2

#define UP_TRAILER_FAIL_ON_STATE     2
#define UP_TRAILER_FAIL_OFF_STATE    3
#define DOWN_TRAILER_FAIL_ON_STATE   4
//#define DOWN_TRAILER_FAIL_OFF_STATE  5

#define CHECK_STEPS 200//количество шагов необходимое для гарантированного слабатывания концевиков

//_____________________________________________________________________________________
// Объявление переменных

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// определение типов

typedef struct {
    
    struct {//заполняется при запуске контроллера в функции SetDevSettings()
        unsigned short int MaxSpeed;
        unsigned short int TransferCoefficientSL;//шаг/мм
        unsigned short int MaxControlVolume;
    } DeveloperSettings;//настройки установленные на этапе настройки оборудования

    struct {//заполняется при запуске контроллера в функции SetDevSettings()
        unsigned short int TransferCoefficientLV;//мм/мл
        unsigned short int ControlVolume;
        unsigned short int MaxSpeedOil;
        unsigned short int EndPhasePoint[5];
        unsigned short int PhaseSpeed[5];
    } DefaultPresets;//настройки установленные на этапе настройки оборудования

    struct {//заполняется при инициализации в функции InitStepMotor().
        unsigned short int TransferCoefficientLV;
        unsigned short int ControlVolume;
        unsigned short int MaxSpeedOil;
        unsigned short int EndPhasePoint[5];
        unsigned short int PhaseSpeed[5];
    } Presets; //Структура заданных настроек //изменяется - CommandStepMotor()

    struct {//заполняется при инициализации в функции InitStepMotor().
        volatile unsigned char MovingState;//состояние движения                               //изменяется - CommandStepMotor(),SmTrailerReaction(void)
        volatile unsigned char Direction;//направление текущего движения                      //изменяется - CommandStepMotor(),SmTrailerReaction(void),StepReaction(void)
        volatile unsigned short int Speed;//скорость текущего движения                        //изменяется - CommandStepMotor(),SmTrailerReaction(void),StepReaction(void)
        volatile signed long TargetPosition;//конечная точка текущего движения                //изменяется - CommandStepMotor(),SmTrailerReaction(void),StepReaction(void)
    } MovingSettings; //Структура настроек текущего движения - отображает настройки текущего движения ШД.//изменяется CommandStepMotor(),SmTrailerReaction(void),StepReaction(),

    struct {
        volatile unsigned char WorkMode;//Режим работы ШД //изменяется - CommandStepMotor(),SmTrailerReaction()
        unsigned char Zero;
        unsigned char Block;//
        volatile unsigned char NeedStepReaction;//флаг ожидания реакции на выполненный шаг       //изменяется - _T2Interrupt(void),StepReaction(void)
        volatile signed long GeneralStepCounter;//счетчик шагов от нулевой позиции               //изменяется - CommandStepMotor(),SmTrailerReaction(),_T2Interrupt(void)
        volatile unsigned short int MovingStepCounter;//счетчик шагов текущего движения                //изменяется - CommandStepMotor(),SmTrailerReaction(),_T2Interrupt(void)      
    } State;//Структура состояния ШД - отображает текущее состояние ШД.//изменяется - CommandStepMotor(), _T2Interrupt(void),SmTrailerReaction()

    struct {
        volatile unsigned char Down;
        volatile unsigned char DownBlock;
        volatile unsigned char Up;
        volatile unsigned char UpBlock;
        unsigned char Alarm;
    }Trailer;

} SM_t;
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Объявление переменных
extern volatile SM_t StepMotor;

//_____________________________________________________________________________________
// Объявление функций

void ConfigPortsStepMotor(void);
void InitStepMotor(void);
void CommandStepMotor(void);
void SmTrailerReaction(void);
void StepReaction(void);
void CheckStepMotorState(void);
void AlarmReactionStepMotor(void);
//_____________________________________________________________________________________
// Определение макросов

#define StepMotor_On()	OnTimer2();\
                        SetFlagTimer2Int();
                        
#define StepMotor_Off() StepMotor.MovingSettings.Direction    = STOP;\
                        StepMotor.MovingSettings.Speed        = STOP;\
                        StepMotor.MovingSettings.TargetPosition = STOP;
                                        
#endif//END #ifndef _STEPMOTOR_H

