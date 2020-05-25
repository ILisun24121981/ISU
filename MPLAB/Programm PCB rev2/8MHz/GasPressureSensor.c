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
    // �������� ����� ������� ��������
    GAS_PRESSURE_TRIS	=1; // ���� �� ����
    GAS_PRESSURE_OD     =0; // �������� ���������
    GAS_PRESSURE_CNIE	=1; // ������������ � �������� ��������� ���������� CN
    GAS_PRESSURE_CNPUE	=0; // �������� �����
    GAS_PRESSURE_CNPDE	=0; // �������� ����

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //������� ��������� ��������� ��
    GasPressureSensor.Block = ON;

    // ������������ ����������� ����� �������, ��� ��� ��������� ��������� ������� �� ����� ����� �������� 0 (=> 0-�������,1 - ���������)
    // ������ �������� ��������� ���������! => ��� ��������  = ��������� = 1 = ALARM
    if (GAS_PRESSURE_PORT == 1) {//��������� = ALARM
        GasPressureSensor.CurErrorState = SETTING_ALARM;
    } else {//������� = NORMAL
        GasPressureSensor.CurErrorState = SETTING_NORMAL;
    }
    GasPressureSensorReaction();
}
//_____________________________________________________________________________________
void GasPressureSensorReaction(void) {

    switch (GasPressureSensor.CurErrorState) {
        case SETTING_ALARM://��������� �������� � ��������� ALARM.
            DisableCnInt();
            if (GAS_PRESSURE_PORT == 1) {//������� ��������� ALARM => ������� ���������
                GasPressureSensor.CurErrorState = ALARM;
                EnableCnInt();
                if(GasPressureSensor.Block == 1){
                    //������ ������ ������� - ������� ��������
                    if (Reason == NO) {
                        Reason = GAS_PRESSURE_SENSOR_ALARM;
                    }
                    //��������� ��������
                    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
                    //��������� SM1150
                    SM1150_Off();
                    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
                    //������������� ��
                    AlarmReactionStepMotor();
                    AlarmReactionPneumatics();
                    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
                    // ������� ��������
                }
            } else {//������� ��������� NORMAL => ������� �� ���������
                GasPressureSensor.CurErrorState = NORMAL;
                EnableCnInt();
            }
            break;
        case SETTING_NORMAL://��������� �������� � ��������� NORMAL.
            DisableCnInt();
            if (GAS_PRESSURE_PORT == 0) {//������� ��������� NORMAL => ������� ���������
                GasPressureSensor.CurErrorState = NORMAL;
                EnableCnInt();
            } else {//������� ��������� ALARM => ������� �� ���������
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
