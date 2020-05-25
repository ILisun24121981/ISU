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

#define	FCY	16000000  //тактовая частота
//_____________________________________________________________________________________
//  Объявление "глобальных" констант 

#define ON              1
#define OFF             0

#define DET             1
#define NOT_DET		0

#define YES             1
#define NO		0

#define ENABLE_STATE    1
#define DISABLE_STATE   0

#define START           1 //начало процесса
#define STOP            0 //конец процесса, процесс не активен, процесс закончен
#define WAIT            2 //ожидание

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

//константы причины произошедшего события(в случае возникновения тревоги).Используются в функциях реакции на тревогу присваиваются переменной Reason
#define SM1150_ALARM                        1
#define GAS_PRESSURE_SENSOR_ALARM           2
#define GAS_PLASMA_FLOW_SENSOR_ALARM        3
#define GAS_TRANSPORT_FLOW_SENSOR_ALARM     4
#define GAS_EXTRACT_SENSOR_ALARM            5
#define PMT_SENSOR_OFF                      6

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Объявление переменных
extern unsigned char Reason;//переменная отслеживающая тревогу которая привела к отклонению процесса анализа от стандартного протекания (тревога по причине которой остановился анализ)
//_____________________________________________________________________________________
//  Объявление функций
void InitPic(void);
void AnalyseRxPacket(void);//функция анализа обнаруженного принятого пакета
void ReasonCommand(void);//
void CheckModuleState(void);
void __attribute__((interrupt, no_auto_psv)) _AddressError (void);
void __attribute__((interrupt, no_auto_psv)) _StackError (void);
void __attribute__((interrupt, no_auto_psv)) _MathError (void);
void __attribute__((interrupt, no_auto_psv)) _OscillatorFail (void);

#endif //END #ifndef _MAIN_H
//_____________________________________________________________________________________
//HELP программисту

//1. Сокращенные  и полные названия битов или группы битов (# _TRISD3  - сокращенное # U2MODEbits.PDSEL0, U2MODEbits.PDSEL  - полное)
/*


Для использования полных и/или сокращенных имен см pic24fj128GA106.h - файл находится в папке компиллятора С30 раздел Support.
Поиском в этом файле находишь описание типа # для U1MODE

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

Соответственно теперь мы знаем, что можно использовать
полное имя:
		для бита PDSEL0  - U2MODEbits.PDSEL0
		для группы битов PDSEL0 и PDSEL1- U2MODEbits.PDSEL


Для использования сокращенных имен
Поиском в этом файле находишь нужный регистр # для TRISD

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

Соответственно теперь мы знаем, что можно использовать
сокращенное имя:
		для бита TRISD0(полное имя TRISDbits.TRISD0) - _TRISD0


результат поиска описания сокращенных имен # для U1MODE

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

Eсли написанно что данный бит (группа битов) не может иметь сокращенное название так как используется одинаковое название бита
в разных регистрах SFR,  то нет сокращенных названий - используем полные.


В тексте программы используются как сокращенные так и полные имена (даже если есть сокращенное для данного полного)
*/
//2. Начальная инициализация работы модулей контроллера (# UART)
/*
После начальной инициализации:
	1.модуль отключен (любая работа модуля DISABLE)
	2.Используемые данным модулем прерывания выключены/запрещены, флаги прерываний очищены.
*/
//3 Настройка приоритетов прерываний
/*
	Для удобства настройки приоритетов прерываний используется отдельная функция InterruptPriority
	Если возникает необходимость добавления еще одного источника прерываний создаем файл описывающий ISR
*/
//4. Начальная инициализация программных модулей (# StepMotor )
/*
	После начальной инициализации:
	1.модуль отключен (любая работа модуля DISABLE)
	2.Выставлены настройки по умолчанию(Default Presets)*/


