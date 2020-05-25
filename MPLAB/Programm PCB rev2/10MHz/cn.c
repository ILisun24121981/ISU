#include "cn.h"

#include "main.h"
//���������� ��� ������ � ������� ������������ ���������� CN
#include "StepMotorProba.h"
#include "StepMotorPMT.h"
#include "SM1150.h"
#include "GasPressureSensor.h"
#include "GasExtractSensor.h"
#include "GasPlasmaFlowSensor.h"
#include "GasTransportFlowSensor.h"


//_____________________________________________________________________________________
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){//���������� ���������� CN
    unsigned short int port;
    ClearFlagCnInt();//�������� ���� ���������� �� ������� CN //�������� � ������ ��������� ����� �� ���������� ���������� ��������� � �������� ���������
// ��������� ��
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // ���������� ������� ��������� ���������� ��
    // ������������ ����������� ����� �������, ��� ��� ��������� ��������� ��������� �� ����� ����� �������� 0 (=> 0-�������,1 - ���������)
    // ������ �������� ��������� �������! =>
    // �������� =  ��������� = 1 = ON (�� �������� = ������� = 0 = OFF) - ������� ����������!
    if(END_TRAILER_PORT == 0){//������� ��������� OFF
        if(StepMotor.Trailer.Down == ON){//�������������� ��������� ON
            StepMotor.Trailer.Down = SETTING_OFF;//��������� �������� �� ��������� ON � ��������� OFF
        }
    }else{//������� ��������� ON
        if(StepMotor.Trailer.Down == OFF){//�������������� ��������� OFF
            StepMotor.Trailer.Down = SETTING_ON;
        }
    }
    if(START_TRAILER_PORT == 0){//������� ��������� OFF
       if(StepMotor.Trailer.Up == ON){//�������������� ��������� ON
            StepMotor.Trailer.Up = SETTING_OFF;
        }
    }else{
        if(StepMotor.Trailer.Up == OFF){//�������������� ��������� OFF
            StepMotor.Trailer.Up = SETTING_ON;
        }
    }

    if(ZERO_TRAILER_PORT == 0){//������� ��������� OFF
        if(StepMotor1.Trailer.State == ON){//�������������� ��������� ON
            StepMotor1.Trailer.State = SETTING_OFF;//��������� �������� �� ��������� ON � ��������� OFF
        }
    }else{//������� ��������� ON
        if(StepMotor1.Trailer.State == OFF){//�������������� ��������� OFF
            StepMotor1.Trailer.State = SETTING_ON;
        }
    }
//SM1150 (������)
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (SM1150.CurPinState != CHANGE) {//�� ������ ���� �� ����� � ������� ��������� ���������� ��� ��� �������� ���������� - ��� ���� ���������!
        port = SM1150_PORT; //��������� ����
        SM1150.CurPinState = port & SM1150_ERROR_MASK; //���������� ������� ��������� �����
        if (SM1150.CurPinState != SM1150.PrevPinState) {//��������� ���������� �� ��� �� �����������
            SM1150.PrevErrorState = SM1150.CurErrorState; 
            SM1150.CurErrorState = CHANGE;//���������� ���������� � ������� SM1150ErrorReaction()
        }
    }
//������ ��������
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (GAS_PRESSURE_PORT == 1) {//��������� = ALARM ������� ���������
        if (GasPressureSensor.CurErrorState == NORMAL) {//�������������� ��������� NORMAL
            GasPressureSensor.CurErrorState = SETTING_ALARM;
        }
    } else {//������� = NORMAL ������� ���������
        if (GasPressureSensor.CurErrorState == ALARM) {//�������������� ��������� ALARM
            GasPressureSensor.CurErrorState = SETTING_NORMAL;
        }
    }

//������ ����������
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (GAS_EXTRACT_PORT == 1) {//��������� = ALARM ������� ���������
        if (GasExtractSensor.CurErrorState == NORMAL) {//�������������� ��������� NORMAL
            GasExtractSensor.CurErrorState = SETTING_ALARM;
        }
    } else {//������� = NORMAL ������� ���������
        if (GasExtractSensor.CurErrorState == ALARM) {//�������������� ��������� ALARM
            GasExtractSensor.CurErrorState = SETTING_NORMAL;
        }
    }
//������ ������ ����������������� ����
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if(GasPlasmaFlowSensor.CurErrorState != CHANGE){//�� ������ ���� �� ����� � ������� ��������� ���������� ��� ��� �������� ���������� - ��� ���� ���������!
        port = GAS_PLASMA_FLOW_SENSOR_PORT;//��������� ����
        GasPlasmaFlowSensor.CurPinState = port & GAS_PLASMA_PIN_STATE_MASK;//���������� ������� ��������� �����
        if(GasPlasmaFlowSensor.CurPinState != GasPlasmaFlowSensor.PrevPinState){//��������� ���������� �� ��� �� �����������
            GasPlasmaFlowSensor.PrevErrorState = GasPlasmaFlowSensor.CurErrorState;
            GasPlasmaFlowSensor.CurErrorState = CHANGE;//���������� ���������� � ������� GasPlasmaFlowSensorReaction()
        }      
    }
//������ ������ ����������������� ����
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if(GasTransportFlowSensor.CurErrorState != CHANGE){//�� ������ ���� �� ����� � ������� ��������� ���������� ��� ��� �������� ���������� - ��� ���� ���������!
        port = GAS_TRANSPORT_FLOW_SENSOR_PORT;//��������� ����
        GasTransportFlowSensor.CurPinState = port & GAS_TRANSPORT_PIN_STATE_MASK;//���������� ������� ��������� �����
        if(GasTransportFlowSensor.CurPinState != GasTransportFlowSensor.PrevPinState){//��������� ���������� �� ��� �� �����������
            GasTransportFlowSensor.PrevErrorState = GasTransportFlowSensor.CurErrorState;
            GasTransportFlowSensor.CurErrorState = CHANGE;//���������� ���������� � ������� GasPlasmaFlowSensorReaction()
        }
    }
}
//_____________________________________________________________________________________
void CnReaction(void){// �������� ��� ������� ���������� ������� �� ���������� CN. ������ ������� ������� � ��������������� ������
    SmTrailerReaction();
    SmTrailerReaction1();
    SM1150ErrorReaction();
    GasPressureSensorReaction();
    GasExtractSensorReaction();
    GasPlasmaFlowSensorReaction();
    GasTransportFlowSensorReaction();
}
