#include "GasTransportFlowSensor.h"

#include "main.h"
#include "cn.h"
#include "StepMotorProba.h"
#include "SM1150.h"
#include "uart.h"
#include "Command.h"
#include "packet.h"

GasTransportFlowSensor_t GasTransportFlowSensor;

//_____________________________________________________________________________________
void InitGasTransportFlowSensor(void) {//функция инициализации датчика
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Настроим порты датчика потока плазмаобразующего газа

    GAS_TRANSPORT_MAX_TRIS = 1; // порт на вход
    GAS_TRANSPORT_MAX_OD = 0; // не открытый коллектор
    GAS_TRANSPORT_MAX_CNIE = 1; // использовать в качестве источника прерывания CN
    GAS_TRANSPORT_MAX_CNPUE = 0; // подтяжка вверх выкл
    GAS_TRANSPORT_MAX_CNPDE = 0; // подтяжка вниз выкл

    GAS_TRANSPORT_MIN_TRIS = 1;
    GAS_TRANSPORT_MIN_OD = 0;
    GAS_TRANSPORT_MIN_CNIE = 1;
    GAS_TRANSPORT_MIN_CNPUE = 0;
    GAS_TRANSPORT_MIN_CNPDE = 0;

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //Зададим начальное состояние ШД
    GasTransportFlowSensor.Block = ON;

    GasTransportFlowSensor.PrevPinState = GAS_TRANSPORT_NORMAL_PIN_STATE;
    GasTransportFlowSensor.CurPinState = GAS_TRANSPORT_NORMAL_PIN_STATE;

    GasTransportFlowSensor.PrevErrorState = NORMAL;
    GasTransportFlowSensor.CurErrorState = CHANGE;

    GasTransportFlowSensorReaction();
}
//_____________________________________________________________________________________
void GasTransportFlowSensorReaction(void) {//функция фиксации изменения состояния датчика и реакции на это изменение
    unsigned short int port;

    if (GasTransportFlowSensor.CurErrorState == CHANGE) {
        DisableCnInt(); //
        port = GAS_TRANSPORT_FLOW_SENSOR_PORT;
        GasTransportFlowSensor.CurPinState = (port & GAS_TRANSPORT_PIN_STATE_MASK);
        if (GasTransportFlowSensor.CurPinState != GasTransportFlowSensor.PrevPinState) {
            GasTransportFlowSensor.PrevPinState = GasTransportFlowSensor.CurPinState;//обязательная строчка - чтобы среагировать на следующее прерывание, если это возврат в состояние из которого осуществлен переход в текущей обработке
            if (GasTransportFlowSensor.CurPinState == GAS_TRANSPORT_NORMAL_PIN_STATE) {
                GasTransportFlowSensor.CurErrorState = NORMAL;
                EnableCnInt();
            } else {//GasPlasmaFlowSensor.CurPinState != GAS_PLASMA_NORMAL_PIN_STATE
                switch (GasTransportFlowSensor.CurPinState) {
                    case GAS_TRANSPORT_ALARM_HIGHT_PIN_STATE:
                        GasTransportFlowSensor.CurErrorState = ALARM_HIGH;
                        EnableCnInt();
                        break;
                    case GAS_TRANSPORT_ALARM_LOW_PIN_STATE:
                        GasTransportFlowSensor.CurErrorState = ALARM_LOW;
                        EnableCnInt();
                        break;
                    case GAS_TRANSPORT_ERROR_PIN_STATE:
                        GasTransportFlowSensor.CurErrorState = ERROR;
                        EnableCnInt();
                        break;
                }
                if(GasTransportFlowSensor.Block == 1){
                    //ставим маркер реакции - причина действий
                    if(Reason == NO){
                        Reason = GAS_TRANSPORT_FLOW_SENSOR_ALARM;
                    }
                    //выполняем действия
                    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
                    //Выключаем SM1150
                    SM1150_Off();
                    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
                    //Останавливаем ШД
                    AlarmReactionStepMotor();
                    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
                    // реакция окончена
                }
            }
        }else{
            GasTransportFlowSensor.CurErrorState = GasTransportFlowSensor.PrevErrorState;
            EnableCnInt();
        }
    }
}
//_____________________________________________________________________________________
void GasTransportFlowSensorCommand(void) {
    if (Uart.Rx.Packet.Command == GAS_TRANSPORT_FLOW_SENSOR_SET_BLOCK) {
        if (GasTransportFlowSensor.Block == NO) {
            GasTransportFlowSensor.Block = YES;
            DisableCnInt();
            GasTransportFlowSensor.PrevPinState = GAS_TRANSPORT_NORMAL_PIN_STATE;
            EnableCnInt();
            if (GasTransportFlowSensor.CurErrorState != CHANGE) {//на случай если уже ждем обработку!
                GasTransportFlowSensor.PrevErrorState = GasTransportFlowSensor.CurErrorState;
                GasTransportFlowSensor.CurErrorState = CHANGE;
            }
        } else {
            GasTransportFlowSensor.Block = NO;
        }
    }
}
