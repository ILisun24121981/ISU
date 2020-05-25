#include "cn.h"

#include "main.h"
//Подключаем все модули и датчики использующие прерывание CN
#include "StepMotorProba.h"
#include "StepMotorPMT.h"
#include "SM1150.h"
#include "GasPressureSensor.h"
#include "GasExtractSensor.h"
#include "GasPlasmaFlowSensor.h"
#include "GasTransportFlowSensor.h"


//_____________________________________________________________________________________
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){//обработчик прерывания CN
    unsigned short int port;
    ClearFlagCnInt();//очистить флаг прерывания по выводам CN //обнуляем в начале обработки чтобы не пропустить прерывание возникшее в процессе обработки
// Концевики ШД
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Определяем текущее состояние концевиков ШД
    // Схемотехника реализована таким образом, что при замкнутом состоянии концевика на входе порта контроля 0 (=> 0-замкнут,1 - разомкнут)
    // Нижний концевик нормально замкнут! =>
    // сработал =  разомкнут = 1 = ON (не сработал = замкнут = 0 = OFF) - верхний аналогично!
    if(END_TRAILER_PORT == 0){//текущее состояние OFF
        if(StepMotor.Trailer.Down == ON){//предшествующее состояние ON
            StepMotor.Trailer.Down = SETTING_OFF;//Состояние перехода из состояния ON в состояние OFF
        }
    }else{//текущее состояние ON
        if(StepMotor.Trailer.Down == OFF){//предшествующее состояние OFF
            StepMotor.Trailer.Down = SETTING_ON;
        }
    }
    if(START_TRAILER_PORT == 0){//текущее состояние OFF
       if(StepMotor.Trailer.Up == ON){//предшествующее состояние ON
            StepMotor.Trailer.Up = SETTING_OFF;
        }
    }else{
        if(StepMotor.Trailer.Up == OFF){//предшествующее состояние OFF
            StepMotor.Trailer.Up = SETTING_ON;
        }
    }

    if(ZERO_TRAILER_PORT == 0){//текущее состояние OFF
        if(StepMotor1.Trailer.State == ON){//предшествующее состояние ON
            StepMotor1.Trailer.State = SETTING_OFF;//Состояние перехода из состояния ON в состояние OFF
        }
    }else{//текущее состояние ON
        if(StepMotor1.Trailer.State == OFF){//предшествующее состояние OFF
            StepMotor1.Trailer.State = SETTING_ON;
        }
    }
//SM1150 (ошибка)
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (SM1150.CurPinState != CHANGE) {//на случай если до входа в функцию обработки прерывания еще раз возникло прерывание - уже ждем обработку!
        port = SM1150_PORT; //считываем порт
        SM1150.CurPinState = port & SM1150_ERROR_MASK; //определяем текущее состояние пинов
        if (SM1150.CurPinState != SM1150.PrevPinState) {//проверяем отличается ли оно от предидущего
            SM1150.PrevErrorState = SM1150.CurErrorState; 
            SM1150.CurErrorState = CHANGE;//необходимо обработать в функции SM1150ErrorReaction()
        }
    }
//Датчик давления
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (GAS_PRESSURE_PORT == 1) {//разомкнут = ALARM текущее состояние
        if (GasPressureSensor.CurErrorState == NORMAL) {//предшествующее состояние NORMAL
            GasPressureSensor.CurErrorState = SETTING_ALARM;
        }
    } else {//замкнут = NORMAL текущее состояние
        if (GasPressureSensor.CurErrorState == ALARM) {//предшествующее состояние ALARM
            GasPressureSensor.CurErrorState = SETTING_NORMAL;
        }
    }

//Датчик вентиляции
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (GAS_EXTRACT_PORT == 1) {//разомкнут = ALARM текущее состояние
        if (GasExtractSensor.CurErrorState == NORMAL) {//предшествующее состояние NORMAL
            GasExtractSensor.CurErrorState = SETTING_ALARM;
        }
    } else {//замкнут = NORMAL текущее состояние
        if (GasExtractSensor.CurErrorState == ALARM) {//предшествующее состояние ALARM
            GasExtractSensor.CurErrorState = SETTING_NORMAL;
        }
    }
//Датчик потока плазмаобразующего газа
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if(GasPlasmaFlowSensor.CurErrorState != CHANGE){//на случай если до входа в функцию обработки прерывания еще раз возникло прерывание - уже ждем обработку!
        port = GAS_PLASMA_FLOW_SENSOR_PORT;//считываем порт
        GasPlasmaFlowSensor.CurPinState = port & GAS_PLASMA_PIN_STATE_MASK;//определяем текущее состояние пинов
        if(GasPlasmaFlowSensor.CurPinState != GasPlasmaFlowSensor.PrevPinState){//проверяем отличается ли оно от предидущего
            GasPlasmaFlowSensor.PrevErrorState = GasPlasmaFlowSensor.CurErrorState;
            GasPlasmaFlowSensor.CurErrorState = CHANGE;//необходимо обработать в функции GasPlasmaFlowSensorReaction()
        }      
    }
//Датчик потока транспортирующего газа
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if(GasTransportFlowSensor.CurErrorState != CHANGE){//на случай если до входа в функцию обработки прерывания еще раз возникло прерывание - уже ждем обработку!
        port = GAS_TRANSPORT_FLOW_SENSOR_PORT;//считываем порт
        GasTransportFlowSensor.CurPinState = port & GAS_TRANSPORT_PIN_STATE_MASK;//определяем текущее состояние пинов
        if(GasTransportFlowSensor.CurPinState != GasTransportFlowSensor.PrevPinState){//проверяем отличается ли оно от предидущего
            GasTransportFlowSensor.PrevErrorState = GasTransportFlowSensor.CurErrorState;
            GasTransportFlowSensor.CurErrorState = CHANGE;//необходимо обработать в функции GasPlasmaFlowSensorReaction()
        }
    }
}
//_____________________________________________________________________________________
void CnReaction(void){// содержит все функции выполнения реакции на прерывания CN. Каждая функция описана в соответствующем модуле
    SmTrailerReaction();
    SmTrailerReaction1();
    SM1150ErrorReaction();
    GasPressureSensorReaction();
    GasExtractSensorReaction();
    GasPlasmaFlowSensorReaction();
    GasTransportFlowSensorReaction();
}
