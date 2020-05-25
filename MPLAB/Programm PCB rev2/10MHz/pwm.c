#include "pwm.h"

#include "SM1150.h"

//_____________________________________________________________________________________
void InitPWM(void){

     __builtin_write_OSCCONL(OSCCON & 0xbf); //Unlock Sequence
        PPS_OC1_PIN = PPS_OC1_FUNC;
     __builtin_write_OSCCONL(OSCCON | 0x40); //Lock Sequence

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Настройка режима работы
        OC1CON1 = 0x0000;
        OC1CON2 = 0x0000;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //Настройка регистров в соответствии с 14.3 Pulse-Width Modulation (PWM) Mode
    //Настроим регистр OC1CON1 //PIC24FJ256GA110 Family Data Sheet (REGISTER 14-1: OCxCON1: OUTPUT COMPARE x CONTROL 1 REGISTER)
        OC1CON1 = OC1CON1_VAL;
        OC1CON2 = OC1CON2_VAL;
        OC1TMR = 0;//счетчик модуля CO1
        PWM_PERIOD_REG = DEFAULT_PWM_PERIOD;
        OffPWM();//PWM_DUTY_CYCLE_REG = 0;  = 0 V на выводе ШИМ
}
