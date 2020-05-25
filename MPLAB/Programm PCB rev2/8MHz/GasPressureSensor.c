#include "GasPressureSensor.h"

#include "main.h"
#include "cn.h"
#include "StepMotorProba.h"
#include "SM1150.h"
#include "Pneumatics.h"
#include "uart.h"
#include "Command.h"
#include "packet.h"

GasPressureSensor_t GasPressureSensor;

//_____________________________________________________________________________________
void InitGasPressureSensor(void){
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Настроим порты датчика давления
    GAS_PRESSURE_TRIS	=1; // порт на вход
    GAS_PRESSURE_OD     =0; // открытый коллектор
    GAS_PRESSURE_CNIE	=1; // использовать в качестве источника прерывания CN
    GAS_PRESSURE_CNPUE	=0; // подтяжка вверх
    GAS_PRESSURE_CNPDE	=0; // подтяжка вниз

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //Зададим начальное состояние ШД
    GasPressureSensor.Block = ON;

    // Схемотехника реализована таким образом, что при замкнутом состоянии датчика на входе порта контроля 0 (=> 0-замкнут,1 - разомкнут)
    // Датчик давления нормально разомкнут! => нет давления  = разомкнут = 1 = ALARM
    if (GAS_PRESSURE_PORT == 1) {//разомкнут = ALARM
        GasPressureSensor.CurErrorState = SETTING_ALARM;
    } else {//замкнут = NORMAL
        GasPressureSensor.CurErrorState = SETTING_NORMAL;
    }
    GasPressureSensorReaction();
}
//_____________________________________________________________________________________
void GasPressureSensorReaction(void) {

    switch (GasPressureSensor.CurErrorState) {
        case SETTING_ALARM://Состояние перехода в состояние ALARM.
            DisableCnInt();
            if (GAS_PRESSURE_PORT == 1) {//текущее состояние ALARM => переход состоялся
                GasPressureSensor.CurErrorState = ALARM;
                EnableCnInt();
                if(GasPressureSensor.Block == 1){
                    //ставим маркер реакции - причина действий
                    if (Reason == NO) {
                        Reason = GAS_PRESSURE_SENSOR_ALARM;
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
                GasPressureSensor.CurErrorState = NORMAL;
                EnableCnInt();
            }
            break;
        case SETTING_NORMAL://Состояние перехода в состояние NORMAL.
            DisableCnInt();
            if (GAS_PRESSURE_PORT == 0) {//текущее состояние NORMAL => переход состоялся
                GasPressureSensor.CurErrorState = NORMAL;
                EnableCnInt();
            } else {//текущее состояние ALARM => переход не состоялся
                GasPressureSensor.CurErrorState = ALARM;
                EnableCnInt();
            }
            break;
    }
}
//_____________________________________________________________________________________
void GasPressureSensorCommand(void) {
    if (Uart.Rx.Packet.Command == GAS_PRESSURE_SENSOR_SET_BLOCK) {
        if (GasPressureSensor.Block == NO) {
            GasPressureSensor.Block = YES;
            GasPressureSensor.CurErrorState = SETTING_ALARM;
        }else{
            GasPressureSensor.Block = NO;
        }
    }
}
