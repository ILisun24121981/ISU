#ifndef _PMT_SENSOR_H
#define _PMT_SENSOR_H

//_____________________________________________________________________________________
// ����������� ��������
#define DEFAULT_AVERAGE_NUMBER_FO_PLASMA_DETECT  50
#define DEFAULT_DECISION_LEVEL  10
#define DEFAULT_HEATING_TIME    10800
//_____________________________________________________________________________________
// ���������� ����������

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ����������� �����
typedef struct{
    unsigned short int DefaultHeatingTime;
    unsigned short int HeatingTime;
    unsigned short int HeatingTimerMSec;
    volatile unsigned short int HeatingTimerSec;
    volatile unsigned char HeatingReady;

    volatile unsigned char Block;
    unsigned short int AverageNumberForPlasmaDetect;   // ���������� ������� ��� �� ������� ������������ ���������� ��� �������� ������� � ������� ������
    unsigned short int DefaultAverageNumberForPlasmaDetect;
    unsigned short int CurLevel;        // ������� ������� (����������
    unsigned short int DecisionLevel;   // ������� �������� ������� � ������� ������ (���� - �����, ���� - �� �����)
    unsigned short int DefaultDecisionLevel;
    unsigned char State;
}PMTSensor_t;

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ���������� ����������
extern PMTSensor_t PMTSensor;       //������ ��������� ������ (�����/ �� �����)

//_____________________________________________________________________________________
// ���������� �������
void InitPMTSensor(void);
void PMTSensorReaction(void);
void PMTSensorCommand(void);
#endif//END #ifndef _PMT_SENSOR_H
