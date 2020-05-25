#ifndef _PWM_H
#define _PWM_H

#include "main.h"
#include "timer.h"
#include "SM1150.h"

//данный модуль используется для установки напряжения отвечающего за уровень мощности СВЧ SM1150
//данное напряжение формируется ШИМ и интегрирующей цепочкой.

//_____________________________________________________________________________________
// Определение констант

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Назначение (маппинг) портов модуля OC1(PWM) (задаем в соответствии с контроллером - для ремаппинга при инициализации в функции InitPWM() )

#define PPS_OC1_FUNC       18                   // PIC24FJ256GA110 Family Data Sheet (TABLE 10-3) output function
#define PPS_OC1_PIN        RPOR6bits.RP12R      // PIC24FJ256GA110 Family Data Sheet //Setting the RPORx register with the listed value assigns that output function to the associated RPn pin.


#define OC1CON1_VAL        0x3C06               // 0011110000000110- см. Комментарий - в конце файла
#define OC1CON2_VAL        0x001F               // 0000000000011111- см. Комментарий - в конце файла

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Конастанты настройки модуля OC1(PWM) для работы   (задаем для инициализации в функции InitUart() )

#define PWM_PERIOD_REG          OC1RS                // регистр периода PWM
#define PWM_DUTY_CYCLE_REG      OC1R                 // регистр заполнения PWM

#define DEFAULT_PWM_PERIOD      50000                 // Период PWM по умолчанию
#define DEFAULT_PWM_DUTY_CYCLE  0                     // Заполнение по умолчанию
//_____________________________________________________________________________________
// Объявление функций
void InitPWM(void);

//_____________________________________________________________________________________
// Определение макросов

#define OffPWM()        PWM_DUTY_CYCLE_REG = 0;
                        
#define OnPWM()         PWM_DUTY_CYCLE_REG = SM1150.Power_Level;
                        

#endif//END #ifndef _PWM_H

//Комментарии//_____________________________________________________________________________________
/*
 *  Коментарий OC1CON1_VAL//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //OC1CON1 //PIC24FJ256GA110 Family Data Sheet (REGISTER 14-1: OCxCON1: OUTPUT COMPARE x CONTROL 1 REGISTER)
 * -U                       (POR=0)    = 0;
 * -U                       (POR=0)    = 0;
 * OC1CON1bits.OCSIDL       (POR=0)    = 1;    //Output Compare x halts in CPU Idle mode
 * OC1CON1bits.OCTSEL2      (POR=0)    = 1;                                                             
 * OC1CON1bits.OCTSEL1      (POR=0)    = 1;                                                             
 * OC1CON1bits.OCTSEL0      (POR=0)    = 1;                                                             
    // OC1CON1bits.OCTSEL<2:0> =3;             //Peripheral Clock (FCY)                                 
 * -U                       (POR=0)    = 0;
 * -U                       (POR=0)    = 0;
 * OC1CON1bits.ENFLT0       (POR=0)    = 0;    // Fault 0 input is disabled
 * -U                       (POR=0)    = 0;
 * -U                       (POR=0)    = 0;
 * OC1CON1bits.OCFLT0       (POR=0)    = 0;    // No PWM Fault condition has occurred (this bit is only used when OCM<2:0> = 111)
 * OC1CON1bits.TRIGMODE     (POR=0)    = 0;    // TRIGSTAT is only cleared by software
 * OC1CON1bits.OCM2         (POR=0)    = 1;
 * OC1CON1bits.OCM1         (POR=0)    = 1;
 * OC1CON1bits.OCM0         (POR=0)    = 0;
    // OC1CON1bits.OCM<2:0> = 6;               // Edge-Aligned PWM mode on OCx
*
 *  Коментарий OC1CON2_VAL//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
 * OC1CON2bits.FLTMD        (POR=0)    = 0;    // Fault mode is maintained until the Fault source is removed and a new PWM period starts
 * OC1CON2bits.FLTOUT       (POR=0)    = 0;    // PWM output is driven low on a Fault
 * OC1CON2bits.FLTTRIEN     (POR=0)    = 0;    // Pin I/O condition is unaffected by a Fault
 * OC1CON2bits.OCINV        (POR=0)    = 0;    // OCx output is not inverted
 * -U                       (POR=0)    = 0;
 * -U                       (POR=0)    = 0;
 * -U                       (POR=0)    = 0;
 * OC1CON2bits.OC32         (POR=0)    = 0;    // Cascade module operation disabled
 * OC1CON2bits.OCTRIG       (POR=0)    = 0;    // Synchronize OCx with source designated by SYNCSELx bits
 * OC1CON2bits.TRIGSTAT     (POR=0)    = 0;    // Timer source has not been triggered and is being held clear
 * OC1CON2bits.OCTRIS       (POR=0)    = 0;    // Output Compare Peripheral x connected to the OCx pin
 * OC1CON2bits.SYNCSEL4     (POR=0)    = 1;
 * OC1CON2bits.SYNCSEL3     (POR=1)    = 1;
 * OC1CON2bits.SYNCSEL2     (POR=1)    = 1;
 * OC1CON2bits.SYNCSEL1     (POR=0)    = 1;
 * OC1CON2bits.SYNCSEL0     (POR=0)    = 1;
    // OC1CON2bits.SYNCSEL0<4:0> = 0x1F;       // This OC module
 */
