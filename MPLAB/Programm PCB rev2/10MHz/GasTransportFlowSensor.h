#ifndef _GAS_TRANSPORT_FLOW_SENSOR_H
#define _GAS_TRANSPORT_FLOW_SENSOR_H

#include "main.h"
//_____________________________________________________________________________________
//����������� ��������
// ���������� ����� �������� ������� ��������. (��� ��������� ���������� �������� GAS_PLASMA_FLOW_SENSOR_PORT, GAS_PLASMA_PIN_STATE_MASK � ���� ��������� PIN��)
//P_1(���)
#define GAS_TRANSPORT_MIN_TRIS		_TRISB4
#define GAS_TRANSPORT_MIN_LAT		_LATB4
#define GAS_TRANSPORT_MIN_PORT		_RB4
#define GAS_TRANSPORT_MIN_OD		_ODB4
#define GAS_TRANSPORT_MIN_CNIE		_CN6IE
#define GAS_TRANSPORT_MIN_CNPUE		_CN6PUE
#define GAS_TRANSPORT_MIN_CNPDE		_CN6PDE
//P_2(����)
#define GAS_TRANSPORT_MAX_TRIS		_TRISB5
#define GAS_TRANSPORT_MAX_LAT		_LATB5
#define GAS_TRANSPORT_MAX_PORT		_RB5
#define GAS_TRANSPORT_MAX_OD		_ODB5
#define GAS_TRANSPORT_MAX_CNIE		_CN7IE
#define GAS_TRANSPORT_MAX_CNPUE		_CN7PUE
#define GAS_TRANSPORT_MAX_CNPDE		_CN7PDE


#define GAS_TRANSPORT_FLOW_SENSOR_PORT     PORTB//���� �� ������� �������������� ������
#define GAS_TRANSPORT_PIN_STATE_MASK       0x30//0000000000110000 - ������ ����������� �� ����� B5,B4 (max,min) ����� E - 0000000000maxmin0000

//��������� ������� ������� ���������� �� ��������� ������� ������ ����������������� ���� ��������� ����� �������:
//����������� �������� ����� � �������� ������������ ������ ������ ������� (������������ ������ ���� ����������� ����� �������,����� ���� ������� ������������� � ����� �����!)
//��� ������ ����� �� ���������� ����� ���������� ������ ���� (��������� ����������) - �������� unsigned short int ��������������� PIN_STATE
//������� ��� ��������� PIN�� - ��. ������������� ������� � �������
#define GAS_TRANSPORT_ALARM_HIGHT_PIN_STATE       0x10//0000000000010000 - P_1(min)=1(OFF) P_2(max)=0(ON)
#define GAS_TRANSPORT_NORMAL_PIN_STATE            0x30//0000000000110000 - P_1(min)=1(OFF) P_2(max)=1(OFF)
#define GAS_TRANSPORT_ALARM_LOW_PIN_STATE         0x20//0000000000100000 - P_1(min)=0(ON) P_2(max)=1(OFF)
#define GAS_TRANSPORT_ERROR_PIN_STATE             0x00//0000000000000000 - P_1(min)=0(ON) P_2(max)=0(ON)



//_____________________________________________________________________________________
// ���������� ����������
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ����������� �����
typedef struct {
    unsigned char Block;

    volatile unsigned short int CurPinState;
    volatile unsigned short int PrevPinState;
    volatile unsigned char CurErrorState;
    volatile unsigned char PrevErrorState;

}GasTransportFlowSensor_t;

extern GasTransportFlowSensor_t GasTransportFlowSensor;

//_____________________________________________________________________________________
// ���������� �������

void InitGasTransportFlowSensor(void);
void GasTransportFlowSensorReaction(void);
void GasTransportFlowSensorCommand(void);

#endif//END #ifndef _GAS_TRANSPORT_FLOW_SENSOR_H
