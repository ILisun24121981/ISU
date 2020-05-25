#ifndef _GAS_EXTRACT_SENSOR_H        // ��������� ��������� ��������� ����� ������
#define _GAS_EXTRACT_SENSOR_H

#include "main.h"
//_____________________________________________________________________________________
//����������� ��������
// ���������� ����� �������� ������� ��������.
#define GAS_EXTRACT_TRIS		_TRISE5
#define GAS_EXTRACT_LAT			_LATE5
#define GAS_EXTRACT_PORT		_RE5
#define GAS_EXTRACT_OD			_ODE5
#define GAS_EXTRACT_CNIE		_CN63IE
#define GAS_EXTRACT_CNPUE		_CN63PUE
#define GAS_EXTRACT_CNPDE		_CN63PDE

//_____________________________________________________________________________________
// ���������� ����������
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ����������� �����
typedef struct {
    unsigned char Block;
    volatile unsigned char CurErrorState;

}GasExtractSensor_t;

extern GasExtractSensor_t GasExtractSensor;

//_____________________________________________________________________________________
// ���������� �������

void InitGasExtractSensor(void);
void GasExtractSensorReaction(void);
void GasExtractSensorCommand(void);

#endif//END #ifndef _GAS_EXTRACT_SENSOR_H
