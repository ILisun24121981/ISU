#ifndef _PNEUMATICS_H
#define _PNEUMATICS_H

#include "main.h"

//_____________________________________________________________________________________
//Определение констант
// Назначение портов управления клапанами
#define VALVE1_TRIS		_TRISE2
#define VALVE1_LAT		_LATE2
#define VALVE1_PORT		_RE2
#define VALVE1_OD		_ODE2
#define VALVE1_CNIE		_CN60IE
#define VALVE1_CNPUE		_CN60PUE
#define VALVE1_CNPDE		_CN60PDE

#define VALVE2_TRIS		_TRISE3
#define VALVE2_LAT		_LATE3
#define VALVE2_PORT		_RE3
#define VALVE2_OD		_ODE3
#define VALVE2_CNIE		_CN61IE
#define VALVE2_CNPUE		_CN61PUE
#define VALVE2_CNPDE		_CN61PDE

#define VALVE_IN                VALVE1_LAT
#define VALVE_OUT               VALVE2_LAT

#define IN ON
#define OUT OFF

#define DEFAULT_VALVE_IN_TIME   0
#define DEFAULT_VALVE_OUT_TIME  0x64
#define DEFAULT_NIMBER_OF_CYCLES 1
//_____________________________________________________________________________________
// Объявление переменных

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// определение типов
typedef struct{
    unsigned short int DefaultValveInTime;
    unsigned short int DefaultValveOutTime;
    unsigned short int DefaultNumberOfCycles;

    unsigned short int ValveInTime;
    unsigned short int ValveOutTime;
    unsigned short int NumberOfCycles;
    
    volatile unsigned char State;

    volatile unsigned short int TimeCounter;
    volatile unsigned short int CyclesCounter;
}Pneumatics_t;

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Объявление переменных
extern Pneumatics_t Pneumatics;

//_____________________________________________________________________________________
// Объявление функций
void CongigPortsPneumatics(void);
void InitPneumatics(void);
void CommandPneumatics(void);
void CheckPneumaticsState(void);
void AlarmReactionPneumatics(void);
//_____________________________________________________________________________________
// Макросы
#define PneumaticIn()       Pneumatics.State = IN;\
                            Pneumatics.TimeCounter = 0;\
                            VALVE_IN = 1;\
                            VALVE_OUT = 0;\


#define PneumaticOut()      Pneumatics.State = OUT;\
                            Pneumatics.TimeCounter = 0;\
                            VALVE_IN = 0;\
                            VALVE_OUT = 1;
                            


#endif//END #ifndef _ADC_H
