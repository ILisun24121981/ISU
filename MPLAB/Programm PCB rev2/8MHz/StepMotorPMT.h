#ifndef _STEPMOTOR_PMT_H
#define _STEPMOTOR_PMT_H

#include "main.h"//для возможности использования "глобальных" констант (# ON,OFF, STOP)
#include "timer.h"
//_____________________________________________________________________________________
// Определение констант
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Default Presets

#define	DEFAULT_SM1_MOVING_SPEED                    6250//10 шага в сек - для расчета файл Speed - ограничение на ввод максимальной скорости  - максимальная скорость не может быть ниже 2 ш/сек
#define	DEFAULT_SM1_MAX_CONTROL_VOLUME              3000//ограничение на ввод максимального контрольного объема.


//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Назначение портов управления обмотками ШД.
//Обмотка A
#define WINDING_A1_TRIS         _TRISD6
#define WINDING_A1_LAT          _LATD6
#define WINDING_A1_PORT         _RD6
#define WINDING_A1_OD           _ODD6
#define WINDING_A1_CNIE         _CN15IE
#define WINDING_A1_CNPUE	_CN15PUE
#define WINDING_A1_CNPDE	_CN15PDE
//Обмотка Б
#define WINDING_B1_TRIS         _TRISD5
#define WINDING_B1_LAT          _LATD5
#define WINDING_B1_PORT         _RD5
#define WINDING_B1_OD           _ODD5
#define WINDING_B1_CNIE         _CN14IE
#define WINDING_B1_CNPUE	_CN14PUE
#define WINDING_B1_CNPDE	_CN14PDE
//Обмотка В
#define WINDING_C1_TRIS         _TRISD4
#define WINDING_C1_LAT          _LATD4
#define WINDING_C1_PORT         _RD4
#define WINDING_C1_OD           _ODD4
#define WINDING_C1_CNIE         _CN13IE
#define WINDING_C1_CNPUE	_CN13PUE
#define WINDING_C1_CNPDE	_CN13PDE
//Обмотка Г
#define WINDING_D1_TRIS         _TRISD3
#define WINDING_D1_LAT          _LATD3
#define WINDING_D1_PORT         _RD3
#define WINDING_D1_OD           _ODD3
#define WINDING_D1_CNIE         _CN52IE
#define WINDING_D1_CNPUE	_CN52PUE
#define WINDING_D1_CNPDE	_CN52PDE
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Назначение  портов концевиков ШД
// концевик начального положения
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
// Объявление переменных

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// определение типов

typedef struct {

    struct {//заполняется при инициализации в функции InitStepMotor().
        unsigned char MovingState;
        unsigned char Direction;//направление текущего движения
        volatile signed short int TargetPosition;//конечная точка текущего движения
    } MovingSettings; //Структура настроек текущего движения - отображает настройки текущего движения ШД.

    struct {
        unsigned char DeterminatePositionMarker; // маркер говорящий что идет определение положения не позволяет повторно нажать определение позиции
        unsigned char ZeroSetMarker;// маркер говорящий,что было касание концевика и ноль теперь жестко фиксирован ( т.е. при очередном срабатывании концевика обнуление не будет происходить, будет просто остановка)
        unsigned char InSavedPositionMarker;//маркер говорящий,что текущая позиция соответствует позиции записанной в память программ
        signed short int SavedPosition;
        signed short int GeneralStepCounter;//счетчик шагов от нулевой позиции
        unsigned short int MovingStepCounter;//счетчик шагов текущего движения

    } State;//Структура состояния ШД - отображает текущее состояние ШД.

    struct {
        unsigned char State;
    }Trailer;

} SM1_t;
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Объявление переменных
extern volatile SM1_t StepMotor1;

//_____________________________________________________________________________________
// Объявление функций

void ConfigPortsStepMotor1(void);
void InitStepMotor1(void);
void CommandStepMotor1(void);
void SmTrailerReaction1(void);
void StepReaction1(void);
//_____________________________________________________________________________________
// Определение макросов

#define StepMotor1_On()	OnTimer4();

#define StepMotor1_Off() StepMotor1.MovingSettings.Direction    = STOP;\
                        StepMotor1.MovingSettings.TargetPosition = STOP;

#endif//END #ifndef _STEPMOTOR_PMT_H



