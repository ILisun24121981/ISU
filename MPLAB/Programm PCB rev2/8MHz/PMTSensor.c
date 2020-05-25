#include "PMTSensor.h"

#include "adc.h"
#include "main.h"
#include "StepMotorProba.h"
#include "SM1150.h"
#include "uart.h"
#include "Command.h"
#include "packet.h"
#include "Pneumatics.h"
#include "memory.h"

PMTSensor_t PMTSensor;

//_____________________________________________________________________________________
void InitPMTSensor(void){//функция инициализации датчика
    //сделать настройку блокировок чтобы они загружались из памяти программм
    PMTSensor.Block = YES;
    PMTSensor.CurLevel = 0;
    //PMTSensor.DecisionLevel - заполняется из памяти программ в функции InitPMSettings() файла memory.с
    PMTSensor.State = OFF;
}
//_____________________________________________________________________________________
void PMTSensorReaction(void){

    PMTSensor.CurLevel = ADC.ChannelMean[PMT_SENSOR_CHANNEL];

    if(PMTSensor.CurLevel > PMTSensor.DecisionLevel){
        if(PMTSensor.State == OFF){
            PMTSensor.State = ON;
        }
    }else{
        if (PMTSensor.State == ON){
            if (T3CONbits.TON == 0) {//не идет цикл работы клапанов
                if (Pneumatics.CyclesCounter == 0){
                    if (Pneumatics.State == OUT){   //закончен, чтобы не отрубить подачу высокого если еще остались попытки поджига
                        PMTSensor.State = OFF;
                        if (PMTSensor.Block == YES) {                          
                            if (Reason == NO) {
                                Reason = PMT_SENSOR_OFF;
                            }
                            //выполняем действия
                            //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
                            //                if (T3CONbits.TON == 0) {//прекратили циклы работы клапанов  - на всякий случай :-)
                            //                    Pneumatics.CyclesCounter = 0;
                            //                    PneumaticOut();
                            //                }
                            //Выключаем SM1150
                            SM1150_Off();
                            //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
                            AlarmReactionStepMotor();
                            
                            //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
                            // реакция окончена
                        }
                    }
                }
            }
        }else{
            if(PMTSensor.Block == YES){
                if(Pneumatics.CyclesCounter == 0){
                    if(Pneumatics.State == OUT){
                //if(T3CONbits.TON == 0){
//                        if (Reason == NO) {
//                            Reason = PMT_SENSOR_OFF;
//                        }
                        SM1150_Off();
//                        if(StepMotor.State.Block == OFF){
//                            StepMotor.State.Block = ON;
//                            AlarmReactionStepMotor();
//                        }
                    }
                }
            }
        }
    }
}
//_____________________________________________________________________________________
void PMTSensorCommand(void){
    if (Uart.Rx.Packet.Command == PMT_SENSOR_SET_BLOCK) {
        if(PMTSensor.Block == YES ){
//            if(SM1150.EnableState == ENABLE_STATE){
//                //Pneumatics.NumberOfCycles = 1;
//            }
            PMTSensor.Block = NO;
        }else{
            //Pneumatics.NumberOfCycles = Pneumatics.DefaultNumberOfCycles;
            PMTSensor.Block = YES;

            if (StepMotor.State.Block == OFF) {
                StepMotor.State.Block = ON;
            }
            if(PMTSensor.State == OFF){
                if (Reason == NO) {
                    Reason = PMT_SENSOR_OFF;
                }
                AlarmReactionStepMotor();
            }
        }
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == PMT_SENSOR_SET_AVERAGE_NUMBER){
        StartReadRxData();
        ReadDataFromRxPacket(INT);
        PMTSensor.AverageNumberForPlasmaDetect = Uart.Rx.RxPackDataToRead.Int[0];
        if(PMTSensor.AverageNumberForPlasmaDetect > SIZE_OF_ADC_BUFFER){
            PMTSensor.AverageNumberForPlasmaDetect = SIZE_OF_ADC_BUFFER;
        }
        if(PMTSensor.AverageNumberForPlasmaDetect == 0){
            PMTSensor.AverageNumberForPlasmaDetect = 1;
        }
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == PMT_SENSOR_SET_DECISION_LEVEL) {
        StartReadRxData();
        ReadDataFromRxPacket(INT);
        PMTSensor.DecisionLevel = Uart.Rx.RxPackDataToRead.Int[0];
        if(PMTSensor.DecisionLevel > 0x3FF){//так как максимальное значение АЦП - 0x3FF
           PMTSensor.DecisionLevel = 0x3FF;
        }
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SAVE_FIRE_SETTINGS) {
        SourseAddr.Value = PM_DEF_SETTINGS_PAGE_ADDRESS;
        DataSize = DATA_SIZE_DEF;
        BufferType = DEF_SETTINGS;
        SRbits.IPL = 7;//отключить пользовательские прерывания
        RdPmToTempBuff(SourseAddr,DataSize,BufferType);


        DefSetBuffer.Value.PMTSensor_DecisionLevel = PMTSensor.DecisionLevel;
        DefSetBuffer.Value.PMTSensor_AverageNumberForPlasmaDetect = PMTSensor.AverageNumberForPlasmaDetect;

        ErasePage(SourseAddr.Words.HW, SourseAddr.Words.LW);
        WrTempBuffToPm(SourseAddr,DataSize,BufferType);
        RdPmToTempBuff(SourseAddr,DataSize,BufferType);

        PMTSensor.DefaultDecisionLevel = DefSetBuffer.Value.PMTSensor_DecisionLevel;
        PMTSensor.DefaultAverageNumberForPlasmaDetect = DefSetBuffer.Value.PMTSensor_AverageNumberForPlasmaDetect;

        PMTSensor.DecisionLevel = PMTSensor.DefaultDecisionLevel;
        PMTSensor.AverageNumberForPlasmaDetect = PMTSensor.DefaultAverageNumberForPlasmaDetect;

        SRbits.IPL = 0;//разрешим пользовательские прерывания
    }

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // команды прогрева ФЭУ
    if(Uart.Rx.Packet.Command == SAVE_FIRE_SETTINGS){

        SourseAddr.Value = PM_DEF_SETTINGS_PAGE_ADDRESS;
        DataSize = DATA_SIZE_DEF;
        BufferType = DEF_SETTINGS;
        SRbits.IPL = 7;//отключить пользовательские прерывания
        RdPmToTempBuff(SourseAddr,DataSize,BufferType);

        DefSetBuffer.Value.PMTSensor_HeartingTime = PMTSensor.HeatingTime;

        ErasePage(SourseAddr.Words.HW, SourseAddr.Words.LW);
        WrTempBuffToPm(SourseAddr,DataSize,BufferType);
        RdPmToTempBuff(SourseAddr,DataSize,BufferType);

        PMTSensor.DefaultHeatingTime = DefSetBuffer.Value.PMTSensor_HeartingTime;
        PMTSensor.HeatingTime = PMTSensor.DefaultHeatingTime;

        SRbits.IPL = 0;//разрешим пользовательские прерывания
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == LOAD_FIRE_SETTINGS) {
        PMTSensor.DecisionLevel = PMTSensor.DefaultDecisionLevel;
         PMTSensor.AverageNumberForPlasmaDetect = PMTSensor.DefaultAverageNumberForPlasmaDetect;
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == LOAD_FIRE_SETTINGS) {
        PMTSensor.HeatingTime = PMTSensor.DefaultHeatingTime;
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if(Uart.Rx.Packet.Command == PMT_SET_HEARTING_TIME){

        StartReadRxData();
        ReadDataFromRxPacket(INT);
        PMTSensor.HeatingTime = Uart.Rx.RxPackDataToRead.Int[0];
        PMTSensor.HeatingTimerSec = PMTSensor.HeatingTime;
        PMTSensor.HeatingReady = NO;
        SM1150.EnableState = DISABLE_STATE;
     
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if(Uart.Rx.Packet.Command == PMT_STOP_HEARTING){
        PMTSensor.HeatingReady = YES;
    }

}
