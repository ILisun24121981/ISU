#include "timer.h"

#include "main.h"
#include "StepMotorProba.h"
#include "StepMotorPMT.h"
#include "Pneumatics.h"
#include "PMTSensor.h"
#include "Pneumatics.h"
#include "SM1150.h"
#include "StepMotorPMT.h"
#include "US.h"

volatile unsigned int MainPacketDelay = MAIN_PACKET_DALAY;//задержка отправки основного пакета(ms - Timer1Interrupt)

//_____________________________________________________________________________________
void InitTimers(void){//функция инициализации таймеров
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    T1CON = 0x0000;
    T1CONbits.TSIDL = 1;
    T1CONbits.TGATE = 0;
    T1CONbits.TCKPS = 0;
    T1CONbits.TCS = 0;
    PR1 = 16000; // T = 1 ms (Fcy = 16MHz)
    ClearFlagTimer1Int();
    DisTimer1Int();
    TMR1 = 0;
    OffTimer1();
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    T2CON = 0x0000;//таймер ШД пробы - скорость определяется значением PR2
    T2CONbits.T32 = 0;
    T2CONbits.TSIDL = 1;
    T2CONbits.TGATE = 0;
    T2CONbits.TCKPS = 0b11; //k=256 (Fcy = 16MHz) => частота заполнения TMR2(Ftmr2) = Fcy/K = 62,500kHz => интервал заполнения TMR2 (Ttmr2)=1/Ftmr2 = 16us
    //PR2 = 6250;//количество интервалов (Ttmr2=16us) на один шаг => Tшага = PR2*Ttmr2 (RP2 =6250 => Tшага =0.1c) - изменение данного регистра - изменение скорости ШД - см. файл  Speed
    //значения записываемые в PR2 инициализируется в функции InitPMSettings() до инициализации модулей Timer - переменные стректуры StepMotor.Presets
    T2CONbits.TCS = 0;
    ClearFlagTimer2Int();
    DisTimer2Int();
    TMR2 = 0;
    OffTimer2();
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    T3CON = 0x0000;
    T3CONbits.TSIDL = 1;
    T3CONbits.TGATE = 0;
    T3CONbits.TCKPS = 0b01;//k=8
    T3CONbits.TCS = 0;
    PR3 = 20000;//10ms
    ClearFlagTimer3Int();
    DisTimer3Int();
    TMR3 = 0;
    OffTimer3();
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    T4CON = 0x0000;//таймер ШД - скорость определяется значением PR2
    T4CONbits.T32 = 0;
    T4CONbits.TSIDL = 1;
    T4CONbits.TGATE = 0;
    T4CONbits.TCKPS = 0b11;         //k=256 (Fcy = 16MHz) => частота заполнения TMR2(Ftmr2) = Fcy/K = 62,500kHz => интервал заполнения TMR2 (Ttmr2)=1/Ftmr2 = 16us
    PR4 = DEFAULT_SM1_MOVING_SPEED; //PR2 = 6250;//количество интервалов (Ttmr2=16us) на один шаг => Tшага = PR2*Ttmr2 (RP2 =6250 => Tшага =0.1c) - изменение данного регистра - изменение скорости ШД - см. файл  Speed
    T4CONbits.TCS = 0;
    ClearFlagTimer4Int();
    DisTimer4Int();
    TMR4 = 0;
    OffTimer4();
}
//_____________________________________________________________________________________
void EnableTimer1(void){
    ClearFlagTimer1Int();
    EnTimer1Int();
    OnTimer1();
}
//_____________________________________________________________________________________
void EnableTimer2(void){
    ClearFlagTimer2Int();
    EnTimer2Int();
    OnTimer2();
}
//_____________________________________________________________________________________
void EnableTimer3(void){
    ClearFlagTimer3Int();
    EnTimer3Int();
    OnTimer3();
}
//_____________________________________________________________________________________
void EnableTimer4(void){
    ClearFlagTimer4Int();
    EnTimer4Int();
    OnTimer4();
}
//_____________________________________________________________________________________
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void){//T=1ms
    ClearFlagTimer1Int();
    if(MainPacketDelay !=0){
        MainPacketDelay--;
    }
    if(US.OffMarker == ON){
        US.OffDelayCounterMSec++;
        if(US.OffDelayCounterMSec == 1000){
            US.OffDelayCounterMSec = 0;
            US.OffDelayCounterSec --;
            if(US.OffDelayCounterSec == 0){
                US.OffDelayCounterSec = US_OFF_DELAY;
                US.OffMarker = OFF;
                US_Off();
            }
        }
    }
    if(PMTSensor.HeatingReady == NO){
        PMTSensor.HeatingTimerMSec++;
        if(PMTSensor.HeatingTimerMSec == 1000){
            PMTSensor.HeatingTimerMSec = 0;
            PMTSensor.HeatingTimerSec--;
            if(PMTSensor.HeatingTimerSec == 0){
                PMTSensor.HeatingReady = YES;
            }
        }
    }
    //ClearFlagTimer1Int();
}
//_____________________________________________________________________________________
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){//T=25us
   static unsigned char prevDirection = STOP;
   int delay1 = 32;//циклов соответствует 2мкс при FCY = 16000000
   int delay2 = 8; //циклов соответствует 500нс при FCY =16000000
   ClearFlagTimer2Int();
   
   if (StepMotor.State.NeedStepReaction == NO) {
        if (StepMotor.MovingSettings.MovingState == STOP) {
            //не делаем следующую фазу
             ENABLE_LAT = 1;
            OffTimer2();
            StepMotor_Off();
            prevDirection = STOP;
            TMR2 = 0;
            //_T2IE = 0;
        } else {
            if(ENABLE_PORT == 1){
                ENABLE_LAT = 0;
                while(delay2!=0){
                    delay2--;
                }
                delay2 = 8;   
            }
            if(prevDirection!=StepMotor.MovingSettings.Direction){
                if (StepMotor.MovingSettings.Direction == FORWARD) {
                    DIR_LAT = 1;
                    prevDirection = FORWARD;
                } else {//StepMotor.Settings.Direction == BACK
                    DIR_LAT = 0;
                    prevDirection = BACK;
                }
                while(delay2!=0){
                    delay2--;
                }
                delay2 = 8;      
            }           
            
            STEP_LAT = 1;
            while(delay1!=0){
                delay1--;
            }
            delay1 = 32;
            STEP_LAT = 0;
            
            StepMotor.State.MovingStepCounter++;
            if (StepMotor.MovingSettings.Direction == FORWARD) {
                StepMotor.State.GeneralStepCounter++;
            } else {
                StepMotor.State.GeneralStepCounter--;
            }
            if (StepMotor.State.GeneralStepCounter == StepMotor.MovingSettings.TargetPosition){
                StepMotor.State.NeedStepReaction = YES;
                if(StepMotor.MovingSettings.MovingState == STOP){
                    StepMotor.State.NeedStepReaction = NO;
                }
            }
        }
    }
   //ClearFlagTimer2Int();
}
//_____________________________________________________________________________________
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){//T=1ms
    ClearFlagTimer3Int();
    Pneumatics.TimeCounter++;
    if(Pneumatics.State == IN){
        if(PMTSensor.State == ON){
            if(SM1150.EnableState == ENABLE_STATE){
                Pneumatics.CyclesCounter = 0;
                PneumaticOut();
            }else{
                if (Pneumatics.TimeCounter >= Pneumatics.ValveInTime) {
                    Pneumatics.CyclesCounter--;
                    PneumaticOut();
                }
            }
        }else{
            if(Pneumatics.TimeCounter >= Pneumatics.ValveInTime){
                if(PMTSensor.Block == NO){
                    if(SM1150.EnableState == ENABLE_STATE){
                        Pneumatics.CyclesCounter = 0;
                    }else{
                        Pneumatics.CyclesCounter--;
                    }
                }else{
                    Pneumatics.CyclesCounter--;
                }
                PneumaticOut();
            }
        }
    }else{//Pneumatics.State = OUT
        if(Pneumatics.TimeCounter >= Pneumatics.ValveOutTime){
            if(Pneumatics.CyclesCounter == 0){
                OffTimer3();
                VALVE_OUT = 0;//закрыть обратный клапан
                if(PMTSensor.Block == YES){
                    if(PMTSensor.State == OFF){
                        SM1150_Off();
                    }
                }
            }else{
                PneumaticIn();
            }
        }
    }
}
//_____________________________________________________________________________________

void __attribute__((interrupt, no_auto_psv)) _T4Interrupt(void) {
    static unsigned char stepType1 = 0;
    ClearFlagTimer4Int();
    if (StepMotor1.MovingSettings.MovingState == STOP) {
        //не делаем следующую фазу
        WINDING_A1_LAT = 0;
        WINDING_B1_LAT = 0;
        WINDING_C1_LAT = 0;
        WINDING_D1_LAT = 0;
        OffTimer4();
        StepMotor1_Off();
        TMR4 = 0;

        //_T2IE = 0;
    } else {
        switch (stepType1) {
            case 0:
                WINDING_A1_LAT = 1;
                WINDING_B1_LAT = 0;
                WINDING_C1_LAT = 0;
                WINDING_D1_LAT = 1;
                stepType1++;
                break;
            case 1:
                if (StepMotor1.MovingSettings.Direction == FORWARD1) {
                    WINDING_A1_LAT = 0;
                    WINDING_B1_LAT = 0;
                    WINDING_C1_LAT = 1;
                    WINDING_D1_LAT = 1;
                } else {//StepMotor.Settings.Direction == BACK
                    WINDING_A1_LAT = 1;
                    WINDING_B1_LAT = 1;
                    WINDING_C1_LAT = 0;
                    WINDING_D1_LAT = 0;
                }
                stepType1++;
                break;
            case 2:
                WINDING_A1_LAT = 0;
                WINDING_B1_LAT = 1;
                WINDING_C1_LAT = 1;
                WINDING_D1_LAT = 0;
                stepType1++;
                break;
            case 3:
                if (StepMotor1.MovingSettings.Direction == FORWARD1) {
                    WINDING_A1_LAT = 1;
                    WINDING_B1_LAT = 1;
                    WINDING_C1_LAT = 0;
                    WINDING_D1_LAT = 0;
                    //                        if(StepMotor.MovingSettings.Mode == STOP){
                    //                            StepMotor.State.GeneralStepCounter++;
                    //                        }
                } else {//StepMotor1.Settings.Direction == BACK
                    WINDING_A1_LAT = 0;
                    WINDING_B1_LAT = 0;
                    WINDING_C1_LAT = 1;
                    WINDING_D1_LAT = 1;
                    //                        if(StepMotor.MovingSettings.Mode == STOP){
                    //                            StepMotor.State.GeneralStepCounter--;
                    //                        }
                }
                stepType1 = 0;
                break;
        }
        StepMotor1.State.MovingStepCounter++;
        if (StepMotor1.MovingSettings.Direction == FORWARD1) {
            StepMotor1.State.GeneralStepCounter++;
        } else {
            StepMotor1.State.GeneralStepCounter--;
        }
        if (StepMotor1.State.GeneralStepCounter == StepMotor1.MovingSettings.TargetPosition) {
            StepMotor1.MovingSettings.MovingState = STOP;
            if(StepMotor1.State.DeterminatePositionMarker == YES){
                StepMotor1.State.DeterminatePositionMarker = NO;
            }
        }
    }
}

