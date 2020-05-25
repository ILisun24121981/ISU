#include "SM1150.h"

#include "main.h"
#include "pwm.h"
#include "cn.h"
#include "uart.h"
#include "packet.h"
#include "StepMotorProba.h"
#include "PMTSensor.h"
#include "GasPressureSensor.h"
#include "GasExtractSensor.h"
#include "GasPlasmaFlowSensor.h"
#include "Pneumatics.h"
#include "memory.h"

unsigned char i;
SM1150_t SM1150;
//_____________________________________________________________________________________
void ConfigPortsSM1150(void) {
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Настроим порты контроля ошибок SM1150
    SM1150_CODE_BIT0_TRIS  = 1;
    SM1150_CODE_BIT0_OD    = 0;
    SM1150_CODE_BIT0_CNIE  = 1;
    SM1150_CODE_BIT0_CNPUE = 0;
    SM1150_CODE_BIT0_CNPDE = 0;

    SM1150_CODE_BIT1_TRIS  = 1;
    SM1150_CODE_BIT1_OD    = 0;
    SM1150_CODE_BIT1_CNIE  = 1;
    SM1150_CODE_BIT1_CNPUE = 0;
    SM1150_CODE_BIT1_CNPDE = 0;

    SM1150_CODE_BIT2_TRIS  = 1;
    SM1150_CODE_BIT2_OD    = 0;
    SM1150_CODE_BIT2_CNIE  = 1;
    SM1150_CODE_BIT2_CNPUE = 0;
    SM1150_CODE_BIT2_CNPDE = 0;

    SM1150_CODE_BIT3_TRIS  = 1;
    SM1150_CODE_BIT3_OD    = 0;
    SM1150_CODE_BIT3_CNIE  = 1;
    SM1150_CODE_BIT3_CNPUE = 0;
    SM1150_CODE_BIT3_CNPDE = 0;

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Настроим порты управления SM1150
    SM1150_ON_OFF_TRIS          = 0;
    SM1150_ON_OFF_OD            = 0;
    SM1150_ON_OFF_CNIE          = 0;
    SM1150_ON_OFF_CNPUE         = 0;
    SM1150_ON_OFF_CNPDE         = 0;
    //вывод ответственный за напряжение определяющее мощьность - вывод ШИМ(модуля OC) настраивается в функции InitPWM()
}
//_____________________________________________________________________________________
void InitSM1150(void){
    ConfigPortsSM1150();
    SM1150.EnableState = DISABLE_STATE;
    SM1150_Off();
    InitPWM();//в данной функции устанавливается значение периода ШИМ,но заполнение =0 => на выходе ШИМ 0V
    SM1150_Set_Power();// установим значение заполнения ШИМ определяющее мощьность SM1150 => на выходе ШИМ соответствующий заданной мощности
    SM1150.PrevErrorState = NORMAL_SM1150;
    SM1150.CurErrorState = CHANGE;
    SM1150ErrorReaction();
            
}
//_____________________________________________________________________________________
void CommandSM1150(void) { //функция обработки команд SM1150 c ПК
//
//     if (Uart.Rx.Packet.Command == SM1150_ENABLE_DISABLE) {
//        if (SM1150.EnableState == ENABLE_STATE) {
//            SM1150_Off();
//            SM1150.DisableDirection = ON;
//            SM1150.EnableState = DISABLE_STATE;
//        } else {
//            SM1150.DisableDirection = OFF;
//            //SM1150.EnableState = ENABLE_STATE;
//            if(PMTSensor.Block == OFF){
//                Pneumatics.NumberOfCycles = 1;
//            }
//        }
//    }
    if (Uart.Rx.Packet.Command == SM1150_ENABLE_DISABLE) {
        if(SM1150.Block == OFF){
            if (SM1150.EnableState == ENABLE_STATE) {
                SM1150_Off();
                SM1150.Block = ON;
                SM1150.EnableState = DISABLE_STATE;
            }else{
                SM1150.Block = ON;
            }
        }else{
            SM1150.Block = OFF;
            if(PMTSensor.Block == OFF){
                Pneumatics.NumberOfCycles = 1;
            }
        }
    }

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SM1150_ON) {
        if (SM1150.WorkState == OFF) {
            if (SM1150.CurErrorState == NORMAL_SM1150) {
                if ((GasPressureSensor.CurErrorState == NORMAL) || (GasPressureSensor.Block == OFF)) {
                    if ((GasExtractSensor.CurErrorState == NORMAL) || (GasExtractSensor.Block == OFF)) {
                        if ((GasPlasmaFlowSensor.CurErrorState == NORMAL) || (GasPlasmaFlowSensor.Block == OFF)) {
                            SM1150_On();
                        }
                    }
                }
            }
        } 
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SM1150_OFF) {
        SM1150_Off();
    }

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SM1150_SET_POWER) {
        StartReadRxData();
        ReadDataFromRxPacket(INT);
        if(Uart.Rx.RxPackDataToRead.Int[0] > 50000){
           Uart.Rx.RxPackDataToRead.Int[0] = 50000;
        }
        SM1150.Power_Level = Uart.Rx.RxPackDataToRead.Int[0];
        SM1150_Set_Power() // = OnPWM()команда включения ШИМ перезаписывает значение заполнения ШИМ
    }

    if (Uart.Rx.Packet.Command == SM1150_SET_COEFF) {

        SourseAddr.Value = PM_DEV_SETTINGS_PAGE_ADDRESS;
        DataSize = DATA_SIZE_DEV;
        BufferType = DEV_SETTINGS;
        SRbits.IPL = 7; //отключить пользовательские прерывания
        //считали настройки разработчика из памяти программ в буфер
        RdPmToTempBuff(SourseAddr, DataSize, BufferType);
        SRbits.IPL = 0; //разрешим пользовательские прерывания

        //исправили буфер
        i=0;
        StartReadRxData();
        while(i<24){
           ReadDataFromRxPacket(CHAR);
           DevSetBuffer.Value.SM1150_Coeff[i] = Uart.Rx.RxPackDataToRead.Char[0];
           i++;
        }

        SRbits.IPL = 7; //отключить пользовательские прерывания
        ErasePage(SourseAddr.Words.HW, SourseAddr.Words.LW);
        WrTempBuffToPm(SourseAddr, DataSize, BufferType);
        RdPmToTempBuff(SourseAddr, DataSize, BufferType);
        SRbits.IPL = 0; //разрешим пользовательские прерывания

        i=0;
        while(i<24){
           SM1150.Dev_SM1150_Coeff[i] = DevSetBuffer.Value.SM1150_Coeff[i];
           i++;
        }

    }
    if (Uart.Rx.Packet.Command == SAVE_FIRE_SETTINGS) {
        SourseAddr.Value = PM_DEF_SETTINGS_PAGE_ADDRESS;
        DataSize = DATA_SIZE_DEF;
        BufferType = DEF_SETTINGS;
        SRbits.IPL = 7;//отключить пользовательские прерывания
        RdPmToTempBuff(SourseAddr,DataSize,BufferType);

        DefSetBuffer.Value.SM1150_Power_Level = SM1150.Power_Level;

        ErasePage(SourseAddr.Words.HW, SourseAddr.Words.LW);
        WrTempBuffToPm(SourseAddr,DataSize,BufferType);
        RdPmToTempBuff(SourseAddr,DataSize,BufferType);

        SM1150.Default_Power_Level = DefSetBuffer.Value.SM1150_Power_Level;
        if(SM1150.Default_Power_Level != SM1150.Power_Level){
            SM1150.Power_Level = 0;//ошибка записи
        }

        SRbits.IPL = 0;//разрешим пользовательские прерывания
    }

    if (Uart.Rx.Packet.Command == LOAD_FIRE_SETTINGS) {
        SM1150.Power_Level = SM1150.Default_Power_Level;
    }










//    if (Uart.Rx.Packet.Command == SM1150_WRITE_DEV_SETTINGS) {
//        StartReadRxData();
//        ReadDataFromRxPacket(INT);
//        SM1150.Power_Level = Uart.Rx.RxPackDataToRead.Int[0];
//        SM1150_Set_Power() // = OnPWM()команда включения ШИМ перезаписывает значение заполнения ШИМ
//    }
//
//    if (Uart.Rx.Packet.Command == SM1150_WRITE_DEFAULT_SETTINGS) {
//
//    }
//
//    if (Uart.Rx.Packet.Command == SM1150_SET_POWER) {
//        StartReadRxData();
//        ReadDataFromRxPacket(CHAR);
//        if(Uart.Rx.RxPackDataToRead.Char[0] > SM1150.DeveloperSettings.Max_Power_Level_Number){//принятый НОМЕР уровня мощьности больше максимального (установленного разработчиком)
//            SM1150.Power_Level = SM1150.DeveloperSettings.Power_Level[1][SM1150.DeveloperSettings.Max_Power_Level_Number];
//        }else{//Uart.Rx.RxPackDataToRead.Char[0] < SM1150.DeveloperSettings.Max_Power_Level_Number - принятый НОМЕР уровня мощьности меньше максимального (установленного разработчиком)
//            if(Uart.Rx.RxPackDataToRead.Char[0] < SM1150.DeveloperSettings.Min_Power_Level_Number){//принятый НОМЕР уровня мощьности меньше минимального (установленного разработчиком)
//                SM1150.Power_Level = SM1150.DeveloperSettings.Power_Level[1][SM1150.DeveloperSettings.Min_Power_Level_Number];
//            }else{//SM1150.DeveloperSettings.Min_Power_Level_Number < Uart.Rx.RxPackDataToRead.Char[0] < SM1150.DeveloperSettings.Max_Power_Level_Number
//                SM1150.Power_Level = SM1150.DeveloperSettings.Power_Level[1][Uart.Rx.RxPackDataToRead.Char[0]];
//            }
//        }
//        SM1150_Set_Power() // = OnPWM() - команда включения ШИМ перезаписывает значение заполнения ШИМ значением SM1150.Power_Level
//    }
    
//    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
//    if (Uart.Rx.Packet.Command == SM1150_OFF) {
//        SM1150_Off();
//    }
}
//_____________________________________________________________________________________

void SM1150ErrorReaction(void) {//функция уточнения состояния и обработки реакции на возникновение(изменение) ошибки SM1150  - CN выводы - входит в CnReaction()
    unsigned short int port;


    if (SM1150.CurErrorState == CHANGE) {
        DisableCnInt();
        port = SM1150_PORT;
        SM1150.CurPinState = port & SM1150_ERROR_MASK;
        if (SM1150.CurPinState != SM1150.PrevPinState){
            SM1150.PrevPinState = SM1150.CurPinState;//обязательная строчка - чтобы среагировать на следующее прерывание, если это возврат в состояние из которого осуществлен переход в текущей обработке
            if (SM1150.CurPinState == SM1150_NORMAL_PIN_STATE) {
                SM1150.CurErrorState = NORMAL_SM1150;
                EnableCnInt();
            }else{
                switch (SM1150.CurPinState) {
                    case SM1150_MAGN_PIN_STATE:
                        SM1150.CurErrorState = MAGN;
                        EnableCnInt();
                        break;
                    case SM1150_TEMP_PIN_STATE:
                        SM1150.CurErrorState = TEMP;
                        EnableCnInt();
                        break;
                    case SM1150_ARC_PIN_STATE:
                        SM1150.CurErrorState = ARC;
                        EnableCnInt();
                        break;
                    case SM1150_CURR_PIN_STATE:
                        SM1150.CurErrorState = CURR;
                        EnableCnInt();
                        break;
                    case SM1150_VOLT_PIN_STATE:
                        SM1150.CurErrorState = VOLT;
                        EnableCnInt();
                        break;
                    case SM1150_AIR_PIN_STATE:
                        SM1150.CurErrorState = AIR;
                        EnableCnInt();
                        break;
                    case SM1150_H20_PIN_STATE:
                        SM1150.CurErrorState = H20;
                        EnableCnInt();
                        break;
                    case SM1150_LOCK_PIN_STATE:
                        SM1150.CurErrorState = LOCK;
                        EnableCnInt();
                        break;
                    case SM1150_LEAK_PIN_STATE:
                        SM1150.CurErrorState = LEAK;
                        EnableCnInt();
                        break;
                    case SM1150_RACK_PIN_STATE:
                        SM1150.CurErrorState = RACK;
                        EnableCnInt();
                        break;
                    case SM1150_MREV_PIN_STATE:
                        SM1150.CurErrorState = MREV;
                        EnableCnInt();
                        break;
                    case SM1150_POWD_PIN_STATE:
                        SM1150.CurErrorState = POWD;
                        EnableCnInt();
                        break;
                    case SM1150_RAM_PIN_STATE:
                        SM1150.CurErrorState = RAM;
                        EnableCnInt();
                        break;
                    case SM1150_COM_PIN_STATE:
                        SM1150.CurErrorState = COM;
                        EnableCnInt();
                        break;
                    case SM1150_IMPOSSIBLE_PIN_STATE:
                        SM1150.CurErrorState = CHECK_HARDWARE_BIT0;
                        EnableCnInt();
                        break;
                }
                //ставим маркер реакции - причина действий
                if(Reason == NO){
                    Reason = SM1150_ALARM;
                }
                //выполняем действия
                //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
                //Выключаем SM1150
                SM1150_Off();
                //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
                //Останавливаем ШД
                AlarmReactionStepMotor();
                //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
            }
        }else{
            SM1150.CurErrorState = SM1150.PrevErrorState;
            EnableCnInt();
        }
    }

    if (PMTSensor.HeatingReady == YES) {
        if ((GasPressureSensor.CurErrorState == ALARM) && (GasPressureSensor.Block == YES)) {
            if (SM1150.WorkState == ON) {
                SM1150_Off();
            }
            SM1150.EnableState = DISABLE_STATE;
        } else {
            if (((GasPlasmaFlowSensor.CurErrorState != NORMAL) && (GasPlasmaFlowSensor.CurErrorState != CHANGE)) && (GasPlasmaFlowSensor.Block == YES)) {
                if (SM1150.WorkState == ON) {
                    SM1150_Off();
                }
                SM1150.EnableState = DISABLE_STATE;

            } else {
                if ((GasExtractSensor.CurErrorState == ALARM) && (GasExtractSensor.Block == YES)) {
                    if (SM1150.WorkState == ON) {
                        SM1150_Off();
                    }
                    SM1150.EnableState = DISABLE_STATE;
                } else {
                    if (SM1150.CurErrorState != NORMAL_SM1150) {
                        if (SM1150.WorkState == ON) {
                            SM1150_Off();
                        }
                        SM1150.EnableState = DISABLE_STATE;
                    } else {
                        if (SM1150.Block == OFF) {
                            if (SM1150.EnableState != ENABLE_STATE) {
                                SM1150.EnableState = ENABLE_STATE;
                            }
                        }
                    }
                }
            }
        }
    }
    if ((SM1150.WorkState == OFF)&&(StepMotor.State.Block == ON)) {//если просто выключили SM1150 -не по тревоге
        AlarmReactionStepMotor();
    }
}
void CheckSM1150WorkState(void){
    if(SM1150_ON_OFF_PORT == 1){
        SM1150.WorkState = ON;
    }else{
        SM1150.WorkState = OFF;
    }
}
