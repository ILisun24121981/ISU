#ifndef _GAS_PRESSURE_SENSOR_H
#define _GAS_PRESSURE_SENSOR_H

#include "main.h"
//_____________________________________________________________________________________
//����������� ��������
// ���������� ����� �������� ������� ��������.
#define GAS_PRESSURE_TRIS		_TRISG6
#define GAS_PRESSURE_LAT		_LATG6
#define GAS_PRESSURE_PORT		_RG6
#define GAS_PRESSURE_OD			_ODG6
#define GAS_PRESSURE_CNIE		_CN8IE
#define GAS_PRESSURE_CNPUE		_CN8PUE
#define GAS_PRESSURE_CNPDE		_CN8PDE

//_____________________________________________________________________________________
// ���������� ����������
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ����������� �����
typedef struct {
    unsigned char Block;
    volatile unsigned char CurErrorState;
    
}GasPressureSensor_t;

extern GasPressureSensor_t GasPressureSensor;

//_____________________________________________________________________________________
// ���������� �������

void InitGasPressureSensor(void);
void GasPressureSensorReaction(void);
void GasPressureSensorCommand(void);
#endif//END #ifndef _PRESSURE_SENSOR_H
