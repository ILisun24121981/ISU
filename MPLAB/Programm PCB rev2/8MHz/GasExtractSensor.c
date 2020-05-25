#include "GasExtractSensor.h"

#include "main.h"
#include "cn.h"
#include "StepMotorProba.h"
#include "SM1150.h"
#include "Pneumatics.h"
#include "uart.h"
#include "Command.h"
#include "packet.h"

GasExtractSensor_t GasExtractSensor;

//_____________________________________________________________________________________
void InitGasExtractSensor(void){
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // �������� ����� ������� ��������
    GAS_EXTRACT_TRIS	=1; // ���� �� ����
    GAS_EXTRACT_OD      =0; // �������� ���������
    GAS_EXTRACT_CNIE	=1; // ������������ � �������� ��������� ���������� CN
    GAS_EXTRACT_CNPUE	=0; // �������� �����
    GAS_EXTRACT_CNPDE	=0; // �������� ����

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //������� ��������� ��������� ��
    GasExtractSensor.Block = ON;

    // ������������ ����������� ����� �������, ��� ��������� ���������� �� ����� ����� �������� 1
    if (GAS_EXTRACT_PORT == 1) {// ALARM
        GasExtractSensor.CurErrorState = SETTING_ALARM;
    } else {// NORMAL
        GasExtractSensor.CurErrorState = SETTING_NORMAL;
    }
    GasExtractSensorReaction();
}
//_____________________________________________________________________________________

void GasExtractSensorReaction(void) {
    switch (GasExtractSensor.CurErrorState) {
        case SETTING_ALARM://��������� �������� � ��������� ALARM.
            DisableCnInt();
            if (GAS_EXTRACT_PORT == 1) {//������� ��������� ALARM => ������� ���������
                GasExtractSensor.CurErrorState = ALARM;
                EnableCnInt();
                if(GasExtractSensor.Block == 1){
                    //������ ������ ������� - ������� ��������
                    if (Reason == NO) {
                        Reason = GAS_EXTRACT_SENSOR_ALARM;
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
                GasExtractSensor.CurErrorState = NORMAL;
                EnableCnInt();
            }
            break;
        case SETTING_NORMAL://��������� �������� � ��������� NORMAL.
            DisableCnInt();
            if (GAS_EXTRACT_PORT == 0) {//������� ��������� NORMAL => ������� ���������
                GasExtractSensor.CurErrorState = NORMAL;
                EnableCnInt();
            } else {//������� ��������� ALARM => ������� �� ���������
                GasExtractSensor.CurErrorState = ALARM;
                EnableCnInt();
            }
            break;
    }
}

void GasExtractSensorCommand(void) {
    if (Uart.Rx.Packet.Command == GAS_EXTRACT_SENSOR_SET_BLOCK) {
        if (GasExtractSensor.Block == NO) {
            GasExtractSensor.Block = YES;
            GasExtractSensor.CurErrorState = SETTING_ALARM;
        }else{
            GasExtractSensor.Block = NO;
        }
    }
}
