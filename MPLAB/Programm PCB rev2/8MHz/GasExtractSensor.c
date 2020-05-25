#include "GasExtractSensor.h"

#include "main.h"
#include "cn.h"
#include "StepMotorProba.h"
#include "SM1150.h"
#include "Pneumatics.h"
#include "uart.h"
#include "Command.h"
#include "packet.h"

GasExtractSensor_t GasExtractSensor;

//_____________________________________________________________________________________
void InitGasExtractSensor(void){
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Настроим порты датчика давления
    GAS_EXTRACT_TRIS	=1; // порт на вход
    GAS_EXTRACT_OD      =0; // открытый коллектор
    GAS_EXTRACT_CNIE	=1; // использовать в качестве источника прерывания CN
    GAS_EXTRACT_CNPUE	=0; // подтяжка вверх
    GAS_EXTRACT_CNPDE	=0; // подтяжка вниз

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //Зададим начальное состояние ШД
    GasExtractSensor.Block = ON;

    // Схемотехника реализована таким образом, что отсуствии вентиляции на входе порта контроля 1
    if (GAS_EXTRACT_PORT == 1) {// ALARM
        GasExtractSensor.CurErrorState = SETTING_ALARM;
    } else {// NORMAL
        GasExtractSensor.CurErrorState = SETTING_NORMAL;
    }
    GasExtractSensorReaction();
}
//_____________________________________________________________________________________

void GasExtractSensorReaction(void) {
    switch (GasExtractSensor.CurErrorState) {
        case SETTING_ALARM://Состояние перехода в состояние ALARM.
            DisableCnInt();
            if (GAS_EXTRACT_PORT == 1) {//текущее состояние ALARM => переход состоялся
                GasExtractSensor.CurErrorState = ALARM;
                EnableCnInt();
                if(GasExtractSensor.Block == 1){
                    //ставим маркер реакции - причина действий
                    if (Reason == NO) {
                        Reason = GAS_EXTRACT_SENSOR_ALARM;
                    }
                    //выполняем действия
                    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
                    //Выключаем SM1150
                    SM1150_Off();
                    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
                    //Останавливаем ШД
                    AlarmReactionStepMotor();
                    AlarmReactionPneumatics();
                    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
                    // реакция окончена
                }
            } else {//текущее состояние NORMAL => переход не состоялся
                GasExtractSensor.CurErrorState = NORMAL;
                EnableCnInt();
            }
            break;
        case SETTING_NORMAL://Состояние перехода в состояние NORMAL.
            DisableCnInt();
            if (GAS_EXTRACT_PORT == 0) {//текущее состояние NORMAL => переход состоялся
                GasExtractSensor.CurErrorState = NORMAL;
                EnableCnInt();
            } else {//текущее состояние ALARM => переход не состоялся
                GasExtractSensor.CurErrorState = ALARM;
                EnableCnInt();
            }
            break;
    }
}

void GasExtractSensorCommand(void) {
    if (Uart.Rx.Packet.Command == GAS_EXTRACT_SENSOR_SET_BLOCK) {
        if (GasExtractSensor.Block == NO) {
            GasExtractSensor.Block = YES;
            GasExtractSensor.CurErrorState = SETTING_ALARM;
        }else{
            GasExtractSensor.Block = NO;
        }
    }
}
