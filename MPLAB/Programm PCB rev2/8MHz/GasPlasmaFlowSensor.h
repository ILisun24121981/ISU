#ifndef _GAS_PLASMA_FLOW_SENSOR_H
#define _GAS_PLASMA_FLOW_SENSOR_H

#include "main.h"
//_____________________________________________________________________________________
//Определение констант
// Назначение порта контроля датчика давления. (при изменении необходимо изменить GAS_PLASMA_FLOW_SENSOR_PORT, GAS_PLASMA_PIN_STATE_MASK и коды состояния PINов)
//P_1(мин)
#define GAS_PLASMA_MIN_TRIS		_TRISB2
#define GAS_PLASMA_MIN_LAT		_LATB2
#define GAS_PLASMA_MIN_PORT		_RB2
#define GAS_PLASMA_MIN_OD		_ODB2
#define GAS_PLASMA_MIN_CNIE		_CN4IE
#define GAS_PLASMA_MIN_CNPUE		_CN4PUE
#define GAS_PLASMA_MIN_CNPDE		_CN4PDE
//P_2(макс)
#define GAS_PLASMA_MAX_TRIS		_TRISB3
#define GAS_PLASMA_MAX_LAT		_LATB3
#define GAS_PLASMA_MAX_PORT		_RB3
#define GAS_PLASMA_MAX_OD		_ODB3
#define GAS_PLASMA_MAX_CNIE		_CN5IE
#define GAS_PLASMA_MAX_CNPUE		_CN5PUE
#define GAS_PLASMA_MAX_CNPDE		_CN5PDE


#define GAS_PLASMA_FLOW_SENSOR_PORT     PORTB//порт на котором контролируется датчик
#define GAS_PLASMA_PIN_STATE_MASK	0xC//0000000000001100 - выводы расположены на пинах B3,B2 (max,min) порта B 000000000000maxmin00

//программа анализа выводоа отвечающих за состояние датчика потока плазмообразующего газа построена таким образом:
//считывается значение порта к которому подсоединены данные выводы датчика (схемотехника должна быть реализована таким образом,чтобы биты тревоги располагались в одном порте!)
//При помощи маски из считанного порта выделяются нужные биты (остальные зануляются) - получаем unsigned short int соответствующее PIN_STATE
//Получен код состояния PINов - см. пояснительную записку к датчику
#define GAS_PLASMA_ALARM_HIGHT_PIN_STATE       0x4//0000000000000100 - P_1(min)=1(OFF) P_2(max)=0(ON)
#define GAS_PLASMA_NORMAL_PIN_STATE            0xC//0000000000001100 - P_1(min)=1(OFF) P_2(max)=1(OFF)
#define GAS_PLASMA_ALARM_LOW_PIN_STATE         0x8//0000000000001000 - P_1(min)=0(ON) P_2(max)=1(OFF)
#define GAS_PLASMA_ERROR_PIN_STATE             0x0//0000000000000000 - P_1(min)=0(ON) P_2(max)=0(ON)

//_____________________________________________________________________________________
// Объявление переменных
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// определение типов
typedef struct {
    unsigned char Block;

    volatile unsigned short int CurPinState;
    volatile unsigned short int PrevPinState;
    volatile unsigned char CurErrorState;
    volatile unsigned char PrevErrorState;
    
}GasPlasmaFlowSensor_t;

extern GasPlasmaFlowSensor_t GasPlasmaFlowSensor;

//_____________________________________________________________________________________
// Объявление функций

void InitGasPlasmaFlowSensor(void);
void GasPlasmaFlowSensorReaction(void);
void GasPlasmaFlowSensorCommand(void);

#endif//END #ifndef _GAS_PLASMA_FLOW_SENSOR_H
