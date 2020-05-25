#include "GasPlasmaFlowSensor.h"

#include "main.h"
#include "cn.h"
#include "StepMotorProba.h"
#include "SM1150.h"
#include "Pneumatics.h"
#include "uart.h"
#include "Command.h"
#include "packet.h"

GasPlasmaFlowSensor_t GasPlasmaFlowSensor;

//_____________________________________________________________________________________
void InitGasPlasmaFlowSensor(void) {
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Настроим порты датчика потока плазмаобразующего газа

    GAS_PLASMA_MAX_TRIS = 1; // порт на вход
    GAS_PLASMA_MAX_OD = 0; // не открытый коллектор
    GAS_PLASMA_MAX_CNIE = 1; // использовать в качестве источника прерывания CN
    GAS_PLASMA_MAX_CNPUE = 0; // подтяжка вверх выкл
    GAS_PLASMA_MAX_CNPDE = 0; // подтяжка вниз выкл

    GAS_PLASMA_MIN_TRIS = 1;
    GAS_PLASMA_MIN_OD = 0;
    GAS_PLASMA_MIN_CNIE = 1;
    GAS_PLASMA_MIN_CNPUE = 0;
    GAS_PLASMA_MIN_CNPDE = 0;

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //Зададим начальное состояние ШД
    GasPlasmaFlowSensor.Block = ON;

    GasPlasmaFlowSensor.PrevPinState = GAS_PLASMA_NORMAL_PIN_STATE;
    GasPlasmaFlowSensor.CurPinState = GAS_PLASMA_NORMAL_PIN_STATE;

    GasPlasmaFlowSensor.PrevErrorState = NORMAL;
    GasPlasmaFlowSensor.CurErrorState = CHANGE;

    GasPlasmaFlowSensorReaction();
}
//_____________________________________________________________________________________

void GasPlasmaFlowSensorReaction(void) {
    unsigned short int port;

    if (GasPlasmaFlowSensor.CurErrorState == CHANGE) {
        DisableCnInt(); //
        port = GAS_PLASMA_FLOW_SENSOR_PORT;
        GasPlasmaFlowSensor.CurPinState = (port & GAS_PLASMA_PIN_STATE_MASK);
        if (GasPlasmaFlowSensor.CurPinState != GasPlasmaFlowSensor.PrevPinState) {
            GasPlasmaFlowSensor.PrevPinState = GasPlasmaFlowSensor.CurPinState;//обязательная строчка - чтобы среагировать на следующее прерывание, если это возврат в состояние из которого осуществлен переход в текущей обработке
            if (GasPlasmaFlowSensor.CurPinState == GAS_PLASMA_NORMAL_PIN_STATE) {
                GasPlasmaFlowSensor.CurErrorState = NORMAL;
                EnableCnInt();
            } else {//GasPlasmaFlowSensor.CurPinState != GAS_PLASMA_NORMAL_PIN_STATE
                switch (GasPlasmaFlowSensor.CurPinState) {
                    case GAS_PLASMA_ALARM_HIGHT_PIN_STATE:
                        GasPlasmaFlowSensor.CurErrorState = ALARM_HIGH;
                        EnableCnInt();
                        break;
                    case GAS_PLASMA_ALARM_LOW_PIN_STATE:
                        GasPlasmaFlowSensor.CurErrorState = ALARM_LOW;
                        EnableCnInt();
                        break;
                    case GAS_PLASMA_ERROR_PIN_STATE:
                        GasPlasmaFlowSensor.CurErrorState = ERROR;
                        EnableCnInt();
                        break;
                }
                if(GasPlasmaFlowSensor.Block == 1){
                    //ставим маркер реакции - причина действий
                    if(Reason == NO){
                        Reason = GAS_PLASMA_FLOW_SENSOR_ALARM;
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
            }
        }else{
            GasPlasmaFlowSensor.CurErrorState = GasPlasmaFlowSensor.PrevErrorState;
            EnableCnInt();
        }
    }
}

void GasPlasmaFlowSensorCommand(void) {
    if (Uart.Rx.Packet.Command == GAS_PLASMA_FLOW_SENSOR_SET_BLOCK) {
        if (GasPlasmaFlowSensor.Block == NO) {
            GasPlasmaFlowSensor.Block = YES;
            DisableCnInt();
            GasPlasmaFlowSensor.PrevPinState = GAS_PLASMA_NORMAL_PIN_STATE;
            EnableCnInt();
            if (GasPlasmaFlowSensor.CurErrorState != CHANGE) {//на случай если уже ждем обработку!
                GasPlasmaFlowSensor.PrevErrorState = GasPlasmaFlowSensor.CurErrorState;
                GasPlasmaFlowSensor.CurErrorState = CHANGE;
            }
        } else {
            GasPlasmaFlowSensor.Block = NO;
        }
    }
}
