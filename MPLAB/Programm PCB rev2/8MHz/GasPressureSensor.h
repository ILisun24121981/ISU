#ifndef _GAS_PRESSURE_SENSOR_H
#define _GAS_PRESSURE_SENSOR_H

#include "main.h"
//_____________________________________________________________________________________
//Определение констант
// Назначение порта контроля датчика давления.
#define GAS_PRESSURE_TRIS		_TRISG6
#define GAS_PRESSURE_LAT		_LATG6
#define GAS_PRESSURE_PORT		_RG6
#define GAS_PRESSURE_OD			_ODG6
#define GAS_PRESSURE_CNIE		_CN8IE
#define GAS_PRESSURE_CNPUE		_CN8PUE
#define GAS_PRESSURE_CNPDE		_CN8PDE

//_____________________________________________________________________________________
// Объявление переменных
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// определение типов
typedef struct {
    unsigned char Block;
    volatile unsigned char CurErrorState;
    
}GasPressureSensor_t;

extern GasPressureSensor_t GasPressureSensor;

//_____________________________________________________________________________________
// Объявление функций

void InitGasPressureSensor(void);
void GasPressureSensorReaction(void);
void GasPressureSensorCommand(void);
#endif//END #ifndef _PRESSURE_SENSOR_H
