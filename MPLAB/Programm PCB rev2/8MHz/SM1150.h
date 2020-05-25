#ifndef _SM1150_H
#define _SM1150_H

#include "main.h"
#include "pwm.h"
//_____________________________________________________________________________________
//Определение констант

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Назначение портов управления SM1150.
#define SM1150_ON_OFF_TRIS	_TRISD0
#define SM1150_ON_OFF_LAT	_LATD0
#define SM1150_ON_OFF_PORT	_RD0
#define SM1150_ON_OFF_OD	_ODD0
#define SM1150_ON_OFF_CNIE      _CN49IE
#define SM1150_ON_OFF_CNPUE     _CN49PUE
#define SM1150_ON_OFF_CNPDE     _CN49PDE
//вывод ответственный за напряжение определяющее мощьность - вывод ШИМ(модуля OC) определен в pwm.h
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Назначение портов контроля ошибки SM1150.
#define SM1150_CODE_BIT0_TRIS	_TRISF6
#define SM1150_CODE_BIT0_LAT	_LATF6
#define SM1150_CODE_BIT0_PORT	_RF6
#define SM1150_CODE_BIT0_OD	_ODF6
#define SM1150_CODE_BIT0_CNIE	_CN72IE
#define SM1150_CODE_BIT0_CNPUE	_CN72PUE
#define SM1150_CODE_BIT0_CNPDE  _CN72PDE

#define SM1150_CODE_BIT1_TRIS	_TRISF2
#define SM1150_CODE_BIT1_LAT	_LATF2
#define SM1150_CODE_BIT1_PORT	_RF2
#define SM1150_CODE_BIT1_OD	_ODF2
#define SM1150_CODE_BIT1_CNIE	_CN70IE
#define SM1150_CODE_BIT1_CNPUE	_CN70PUE
#define SM1150_CODE_BIT1_CNPDE  _CN70PDE

#define SM1150_CODE_BIT2_TRIS	_TRISF3
#define SM1150_CODE_BIT2_LAT	_LATF3
#define SM1150_CODE_BIT2_PORT	_RF3
#define SM1150_CODE_BIT2_OD	_ODF3
#define SM1150_CODE_BIT2_CNIE	_CN71IE
#define SM1150_CODE_BIT2_CNPUE	_CN71PUE
#define SM1150_CODE_BIT2_CNPDE  _CN71PDE

#define SM1150_CODE_BIT3_TRIS	_TRISF5
#define SM1150_CODE_BIT3_LAT	_LATF5
#define SM1150_CODE_BIT3_PORT	_RF5
#define SM1150_CODE_BIT3_OD	_ODF5
#define SM1150_CODE_BIT3_CNIE	_CN18IE
#define SM1150_CODE_BIT3_CNPUE	_CN18PUE
#define SM1150_CODE_BIT3_CNPDE  _CN18PDE

#define SM1150_PORT             PORTF
#define SM1150_ERROR_MASK	0x6C//0000000001101100 - выводы расположены на пинах F6,F2,F3,F5 (bit0,bit1,bit2,bit3) порта F

//программа анализа выводоа отвечающих за код ошибки SM1150 построена таким образом:
//считывается значение порта к которому подсоединены данные выводы блока SM1150 (схемотехника должна быть реализована таким образом,чтобы биты тревоги располагались в одном порте!)
//При помощи маски из считанного порта выделяются нужные биты (остальные зануляются) - получаем unsigned short int соответствующее PIN_STATE

//Состояние пинов порта (маскированного)соответствующие кодам ошибок
								//биты числа	   03 21
#define SM1150_NORMAL_PIN_STATE 	0x6C//0000000001101100
#define SM1150_MAGN_PIN_STATE 		0x2C//0000000000101100
#define SM1150_TEMP_PIN_STATE 		0x68//0000000001101000
#define SM1150_ARC_PIN_STATE 		0x28//0000000000101000
#define SM1150_CURR_PIN_STATE 		0x64//0000000001100100
#define SM1150_VOLT_PIN_STATE 		0x24//0000000000100100
#define SM1150_AIR_PIN_STATE 		0x60//0000000001100000
#define SM1150_H20_PIN_STATE 		0x20//0000000000100000
#define SM1150_LOCK_PIN_STATE 		0x4C//0000000001001100
#define SM1150_LEAK_PIN_STATE 		 0xC//0000000000001100
#define SM1150_RACK_PIN_STATE 		0x48//0000000001001000
#define SM1150_MREV_PIN_STATE 		 0x8//0000000000001000
#define SM1150_POWD_PIN_STATE 		0x44//0000000001000100
#define SM1150_RAM_PIN_STATE 		 0x4//0000000000000100
#define SM1150_COM_PIN_STATE		0x40//0000000001000000
#define SM1150_IMPOSSIBLE_PIN_STATE		0x0//0000000000000000

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
//Коды ошибок SM1150 
#define MAGN 		0x1
#define TEMP 		0x2
#define ARC 		0x3
#define CURR 		0x4
#define VOLT 		0x5
#define AIR 		0x6
#define H20 		0x7
#define LOCK 		0x8
#define LEAK 		0x9
#define RACK 		0xA
#define MREV 		0xB
#define POWD 		0xC
#define RAM 		0xD
#define COM		0xE
#define CHECK_HARDWARE_BIT0 0xF

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Константы для наглядности кода SM1150
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Default Presets
#define DEFAULT_SM1150_POWER        0
#define DEFAULT_MIN_POWER_LEVEL     POWER_LEVEL_0
#define DEFAULT_MAX_POWER_LEVEL     POWER_LEVEL_30


//_____________________________________________________________________________________
// Объявление переменных

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// определение типов
typedef struct {
    //настройки разработчика
    unsigned char Dev_SM1150_Coeff[24];//заполняется при запуске контроллера InitPMSettings() и по команде ПК  SM1150_WRITE_DEV_SETTINGS
       
    //настройки по умолчанию
    unsigned short int Default_Power_Level;//Уровень мощьности установленный по умолчанию //заполняется при запуске контроллера InitPMSettings() и по команде ПК в функции SetDevSettings()

    unsigned char EnableState;//блокировка включения - когда OFF включить невозможно
    unsigned char Block;//Указание что блокировка включения была задана вручную пользователем, а не по состоянию тревог по датчикам связанным с работой SM1150
    unsigned char WorkState;//состояник - включен - выключен
    unsigned short int Power_Level;//Уровень мощьности установленный в текущий момент

    volatile unsigned short int CurPinState;//текущее состояние пинов отвечающих за биты ошибки
    volatile unsigned short int PrevPinState;//предидущее состояние пинов отвечающих за биты ошибки
    volatile unsigned char CurErrorState;//текущее состояние ошибки
    volatile unsigned char PrevErrorState;//предидущее состояние ошибки


}SM1150_t;

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Объявление переменных
extern SM1150_t SM1150;
//_____________________________________________________________________________________
// Объявление функций
void ConfigPortsSM1150(void);
void InitSM1150(void);
void CommandSM1150(void);
void SM1150ErrorReaction(void);
void CheckSM1150WorkState(void);
//_____________________________________________________________________________________
// Определение макросов

#define SM1150_On()         if(SM1150.EnableState == ENABLE_STATE){\
                                SM1150_ON_OFF_LAT  = 1;\
                                SM1150.WorkState = ON;\
                            }

#define SM1150_Off()        SM1150_ON_OFF_LAT  = 0;\
                            SM1150.WorkState = OFF;

#define SM1150_Set_Power()  OnPWM();

#endif//END #ifndef _SM1150_H
