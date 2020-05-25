#ifndef _US_H
#define _US_H


#include "main.h"
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Назначение портов управления SM1150.
#define US_ON_OFF_TRIS      _TRISE4
#define US_ON_OFF_LAT       _LATE4
#define US_ON_OFF_PORT      _RE4
#define US_ON_OFF_OD        _ODE4
#define US_ON_OFF_CNIE      _CN62IE
#define US_ON_OFF_CNPUE     _CN62PUE
#define US_ON_OFF_CNPDE     _CN62PDE

#define US_OFF_DELAY        60
//_____________________________________________________________________________________
// Объявление переменных

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// определение типов
typedef struct {
    unsigned char WorkState;
    unsigned char OffMarker; //Маркер запускающий задержку отключения ультразвука
    unsigned int OffDelayCounterMSec;//счетчик времени задержки отключения
    unsigned char OffDelayCounterSec;//счетчик времени задержки отключения
}US_t;

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Объявление переменных
extern US_t US;

//_____________________________________________________________________________________
// Объявление функций
void ConfigPortsUS(void);
void InitUS(void);
void CheckUSWorkState(void);
void CommandUS(void);

//_____________________________________________________________________________________
// Макросы

    #define US_On()         US_ON_OFF_LAT  = 1;\
                            US.WorkState = ON;

    #define US_Off()        US_ON_OFF_LAT  = 0;\
                            US.WorkState = OFF;


#endif//END #ifndef _US_H
