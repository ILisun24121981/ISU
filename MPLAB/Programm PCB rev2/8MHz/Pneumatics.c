#include "Pneumatics.h"

#include "Command.h"
#include "PMTSensor.h"
#include "GasPressureSensor.h"
#include "GasExtractSensor.h"
#include "GasPlasmaFlowSensor.h"
#include "SM1150.h"
#include "packet.h"
#include "memory.h"

Pneumatics_t Pneumatics;
//_____________________________________________________________________________________
void CongigPortsPneumatics(void) {//функция настройки портов клапанов
    // Настройка портов управления клапанами
    // Клапан 1
    VALVE1_TRIS = 0;
    VALVE1_OD = 0;
    VALVE1_CNIE = 0;
    VALVE1_CNPUE = 0;
    VALVE1_CNPDE = 0;

    // Клапан 2
    VALVE2_TRIS = 0;
    VALVE2_OD = 0;
    VALVE2_CNIE = 0;
    VALVE2_CNPUE = 0;
    VALVE2_CNPDE = 0;
}
//_____________________________________________________________________________________
void InitPneumatics(void) { //функция начальной инициализации.
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Настроим порты клапанов
    CongigPortsPneumatics();
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //Зададим начальное состояние клапанов
    //переменные Pneumatics.DefaultValveInTime Pneumatics.DefaultValveOutTime Pneumatics.DefaultNumberOfCycles Pneumatics.ValveInTime Pneumatics.ValveOutTime Pneumatics.NumberOfCycles заполняются из памяти программ в функции InitPMSettings() файла memory.с
    Pneumatics.CyclesCounter = 0;
    PneumaticOut();
    OnTimer3();

}
//_____________________________________________________________________________________
void CommandPneumatics(void){
    if (Uart.Rx.Packet.Command == PNEUMATICS_SET_FIRE) {//запустить цикл работы клапанов (поджог плазмы)
        if (T3CONbits.TON == 0) {
            if (SM1150.EnableState == ENABLE_STATE) {
                if (PMTSensor.HeatingReady == YES){
                    if (SM1150.CurErrorState == NORMAL_SM1150) {
                        if (PMTSensor.State == OFF) {
                            if ((GasPressureSensor.CurErrorState == NORMAL) || (GasPressureSensor.Block == OFF)) {
                                if ((GasExtractSensor.CurErrorState == NORMAL) || (GasExtractSensor.Block == OFF)) {
                                    if ((GasPlasmaFlowSensor.CurErrorState == NORMAL) || (GasPlasmaFlowSensor.Block == OFF)) {
                                        SM1150_On();
                                        Pneumatics.CyclesCounter = Pneumatics.NumberOfCycles;
                                        PneumaticIn();
                                        OnTimer3();
                                    }
                                }
                            }
                        }
                    }
                }
            } else {//SM1150.EnableState = DISABLE - дает возможность настраивать работу клапанов (клапана будут работать без включения SM1150)
                Pneumatics.CyclesCounter = Pneumatics.NumberOfCycles;
                PneumaticIn();
                OnTimer3();
            }
        } else {//остановка (если запущен) цикла работы клапанов
            Pneumatics.CyclesCounter = 0;
            PneumaticOut();
        }
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == PNEUMATICS_SET_SETTINGS) {
        
        StartReadRxData();
        ReadDataFromRxPacket(INT);
        Pneumatics.ValveInTime = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        Pneumatics.ValveOutTime = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        if(PMTSensor.Block == OFF){
            if(SM1150.EnableState == ENABLE_STATE){
                Pneumatics.NumberOfCycles = 1;
            }else{
                Pneumatics.NumberOfCycles = Uart.Rx.RxPackDataToRead.Int[0];
            }
        }else{
            Pneumatics.NumberOfCycles = Uart.Rx.RxPackDataToRead.Int[0];
        }
        if(Pneumatics.NumberOfCycles == 0){
            Pneumatics.NumberOfCycles = 1;
        }
        if(T3CONbits.TON == 0){//если изменяем настройки в процессе работы поджига
            if(Pneumatics.State == IN){
                if(Pneumatics.TimeCounter >= Pneumatics.ValveInTime){
                    PneumaticOut();
                    Pneumatics.TimeCounter = Pneumatics.ValveInTime;
                }
            }
            Pneumatics.CyclesCounter = Pneumatics.NumberOfCycles;
        }
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SAVE_FIRE_SETTINGS) {

        SourseAddr.Value = PM_DEF_SETTINGS_PAGE_ADDRESS;
        DataSize = DATA_SIZE_DEF;
        BufferType = DEF_SETTINGS;
        SRbits.IPL = 7;//отключить пользовательские прерывания
        RdPmToTempBuff(SourseAddr,DataSize,BufferType);

        DefSetBuffer.Value.Pneumatics_ValveInTime = Pneumatics.ValveInTime;
        DefSetBuffer.Value.Pneumatics_ValveOutTime = Pneumatics.ValveOutTime;
        DefSetBuffer.Value.Pneumatics_NumberOfCycles = Pneumatics.NumberOfCycles;

        ErasePage(SourseAddr.Words.HW, SourseAddr.Words.LW);
        WrTempBuffToPm(SourseAddr,DataSize,BufferType);
        RdPmToTempBuff(SourseAddr,DataSize,BufferType);

        Pneumatics.DefaultValveInTime = DefSetBuffer.Value.Pneumatics_ValveInTime;
        Pneumatics.DefaultValveOutTime = DefSetBuffer.Value.Pneumatics_ValveOutTime;
        Pneumatics.DefaultNumberOfCycles = DefSetBuffer.Value.Pneumatics_NumberOfCycles;

        SRbits.IPL = 0;//разрешим пользовательские прерывания
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == LOAD_FIRE_SETTINGS) {

        Pneumatics.ValveInTime = Pneumatics.DefaultValveInTime;
        Pneumatics.ValveOutTime = Pneumatics.DefaultValveOutTime;
        Pneumatics.NumberOfCycles = Pneumatics.DefaultNumberOfCycles;
    }
}
void CheckPneumaticsState(void){
    if(T3CONbits.TON == 0){//клапана не работают - единственно верное состояние OUT
        if((VALVE1_PORT == 0)&&(VALVE2_PORT == 0)){
            Pneumatics.State = OUT;
        }else{
            Pneumatics.CyclesCounter = 0;
            PneumaticOut();
            OnTimer3();
        }
    }
}
void AlarmReactionPneumatics(void){
    if(T3CONbits.TON == 1){
        Pneumatics.CyclesCounter = 0;
        PneumaticOut();
    }
}
