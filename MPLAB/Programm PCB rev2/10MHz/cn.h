#ifndef _CN_H
#define _CN_H

#include "main.h"
//_____________________________________________________________________________________
//Определение констант
    
    #define SETTING_ON            0x10   //Состояние перехода из состояния OFF в состояние ON
    #define SETTING_OFF           0x11   //Состояние перехода из состояния ON в состояние OFF

    #define SETTING_NORMAL        0x12   //Состояние перехода в состояние NORMAL
    #define SETTING_ALARM         0x13   //Состояние перехода в состояние ALARM

//    #define SETTING_ALARM_LOW     0x14  //Состояние перехода в состояние ALARM_LOW
//    #define SETTING_LOW           0x15  //Состояние перехода в состояние LOW
//    #define SETTING_HIGH          0x16  //Состояние перехода в состояние HIGH
//    #define SETTING_ALARM_HIGH    0x17  //Состояние перехода в состояние ALARM_HIGH

    #define CHANGE                0x18  //Изменение состояния - используется в SM1150
    #define SETTING_ERROR         0x19  //Переход в неправильное состояние - свидетельствует о неисправности или неправильной настройке датчика(ов)
//_____________________________________________________________________________________
//  Объявление функций
    void CnReaction(void);
    void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);

//_____________________________________________________________________________________
// Определение макросов
    #define EnableCnInt()     _CNIE = 1; //включить прерывания по выводам CN
    #define DisableCnInt()    _CNIE = 0; //отключить прерывания по выводам CN
    #define SetFlagCnInt()    _CNIF = 1; //усвтановить флаг прерывания по выводам CN
    #define ClearFlagCnInt()  _CNIF = 0; //очистить флаг прерывания по выводам CN
#endif
