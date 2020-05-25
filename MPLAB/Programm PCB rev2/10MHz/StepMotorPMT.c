#include "StepMotorPMT.h"

#include "uart.h"
#include "cn.h"
#include "packet.h"
#include "memory.h"
#include "PMTSensor.h"
#include "StepMotorProba.h"

volatile SM1_t StepMotor1;

//_____________________________________________________________________________________

void CongigPortsStepMotor1(void) {//������� ��������� ������ ��
    // ��������� ������ ���������� ��������� ��
    // ������� A
    WINDING_A1_TRIS = 0;
    WINDING_A1_OD = 0;
    WINDING_A1_CNIE = 0;
    WINDING_A1_CNPUE = 0;
    WINDING_A1_CNPDE = 0;
    // ������� �
    WINDING_B1_TRIS = 0;
    WINDING_B1_OD = 0;
    WINDING_B1_CNIE = 0;
    WINDING_B1_CNPUE = 0;
    WINDING_B1_CNPDE = 0;
    // ������� �
    WINDING_C1_TRIS = 0;
    WINDING_C1_OD = 0;
    WINDING_C1_CNIE = 0;
    WINDING_C1_CNPUE = 0;
    WINDING_C1_CNPDE = 0;
    // ������� �
    WINDING_D1_TRIS = 0;
    WINDING_D1_OD = 0;
    WINDING_D1_CNIE = 0;
    WINDING_D1_CNPUE = 0;
    WINDING_D1_CNPDE = 0;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //��������� ������ ���������� ��
    // ������ �������� (�������� �������)
    ZERO_TRAILER_TRIS = 1;
    ZERO_TRAILER_OD = 0;
    ZERO_TRAILER_CNIE = 1;
    ZERO_TRAILER_CNPUE = 0;
    ZERO_TRAILER_CNPDE = 0;
}//END CongigPortsStepMotor()
//_____________________________________________________________________________________

void InitStepMotor1(void) { //������� ��������� �������������.
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // �������� ����� ��
    CongigPortsStepMotor1();
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //������� ��������� ��������� ��
    StepMotor1.MovingSettings.MovingState = STOP;
    StepMotor1.MovingSettings.Direction = STOP;
    StepMotor1.MovingSettings.TargetPosition = STOP;    
    StepMotor1.State.MovingStepCounter = 0;
    StepMotor1.State.DeterminatePositionMarker = NO;



    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // ������� ��������� ��������� ������ ���������� ��������� ��
    WINDING_A1_LAT = 0;
    WINDING_B1_LAT = 0;
    WINDING_C1_LAT = 0;
    WINDING_D1_LAT = 0;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // ��������� ��������� ��������� ����������
    // ������������ ����������� ����� �������, ��� ��� ��������� ��������� ��������� �� ����� ����� �������� 0 (=> 0-�������,1 - ���������)
    // ������ �������� ��������� ���������! => �������� =  ������� = 0 = ON (�� �������� = ������� = 0 = OFF)
    if (ZERO_TRAILER_PORT == 1) {//��������� = ON
        StepMotor1.Trailer.State = SETTING_ON;
    } else {//��������� = OFF
        StepMotor1.Trailer.State = SETTING_OFF;
    }
    SmTrailerReaction1();
}//END InitStepMotor()


 //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
void CommandStepMotor1(void) { //������� ��������� ������ �� c ��

    if (Uart.Rx.Packet.Command == SM1_DETERMINATE_POSITION) {
        if (StepMotor1.State.InSavedPositionMarker != YES) {
            if (StepMotor1.State.ZeroSetMarker == NO) {
                if(StepMotor1.State.DeterminatePositionMarker == NO){
                    StepMotor1.MovingSettings.MovingState = STOP;
                    while (T4CONbits.TON == 1) {//���� ���� ����� �������� ���
                    };
                    if (StepMotor1.Trailer.State != ON) {
                        StepMotor1.MovingSettings.Direction = BACK1;
                        StepMotor1.MovingSettings.MovingState = MOVING1;
                        StepMotor1.MovingSettings.TargetPosition = StepMotor1.State.GeneralStepCounter - DEFAULT_SM1_MAX_CONTROL_VOLUME;
                        StepMotor1.State.MovingStepCounter = 0;
                        StepMotor1.State.DeterminatePositionMarker = YES;
                        StepMotor1_On();
                    }
                }
            }
        }
    }
    if (Uart.Rx.Packet.Command == SM1_SAVE_POSITION) {
        if (StepMotor1.State.ZeroSetMarker == YES) {
            StepMotor1.MovingSettings.MovingState = STOP;
            while (T4CONbits.TON == 1) {//���� ���� ����� �������� ���
            };
            SourseAddr.Value = PM_DEV_SETTINGS_PAGE_ADDRESS;
            DataSize = DATA_SIZE_DEV;
            BufferType = DEV_SETTINGS;
            SRbits.IPL = 7; //��������� ���������������� ����������
            //������� ��������� ������������ �� ������ �������� � �����
            RdPmToTempBuff(SourseAddr, DataSize, BufferType);
            SRbits.IPL = 0; //�������� ���������������� ����������

            DevSetBuffer.Value.SM1_SavedPosition = StepMotor1.State.GeneralStepCounter;
            DevSetBuffer.Value.SM1_InSavedPositionMarker = YES;

            SRbits.IPL = 7; //��������� ���������������� ����������
            ErasePage(SourseAddr.Words.HW, SourseAddr.Words.LW);
            WrTempBuffToPm(SourseAddr, DataSize, BufferType);
            RdPmToTempBuff(SourseAddr, DataSize, BufferType);
            SRbits.IPL = 0; //�������� ���������������� ����������

            StepMotor1.State.SavedPosition = DevSetBuffer.Value.SM1_SavedPosition;
            StepMotor1.State.InSavedPositionMarker = DevSetBuffer.Value.SM1_InSavedPositionMarker;
        }
    }
    if (Uart.Rx.Packet.Command == SM1_MOVE_FORWARD) {
        if (StepMotor1.State.ZeroSetMarker == YES) {
            StartReadRxData();
            ReadDataFromRxPacket(INT);
            if(Uart.Rx.RxPackDataToRead.Int[0]>0){
                if (StepMotor1.State.GeneralStepCounter < DEFAULT_SM1_MAX_CONTROL_VOLUME) {
                    StepMotor1.MovingSettings.TargetPosition = StepMotor1.State.GeneralStepCounter + (signed short int) Uart.Rx.RxPackDataToRead.Int[0];
                    if (StepMotor1.MovingSettings.TargetPosition > DEFAULT_SM1_MAX_CONTROL_VOLUME) {
                        StepMotor1.MovingSettings.TargetPosition = DEFAULT_SM1_MAX_CONTROL_VOLUME;
                    }

                    StepMotor1.State.MovingStepCounter = 0;
                    StepMotor1.MovingSettings.Direction = FORWARD1;
                    StepMotor1.MovingSettings.MovingState = MOVING1;

                    if (StepMotor1.State.InSavedPositionMarker == YES) {
                        SourseAddr.Value = PM_DEV_SETTINGS_PAGE_ADDRESS;
                        DataSize = DATA_SIZE_DEV;
                        BufferType = DEV_SETTINGS;
                        SRbits.IPL = 7; //��������� ���������������� ����������
                        //������� ��������� ������������ �� ������ �������� � �����
                        RdPmToTempBuff(SourseAddr, DataSize, BufferType);
                        SRbits.IPL = 0; //�������� ���������������� ����������

                        DevSetBuffer.Value.SM1_InSavedPositionMarker = NO;

                        SRbits.IPL = 7; //��������� ���������������� ����������
                        ErasePage(SourseAddr.Words.HW, SourseAddr.Words.LW);
                        WrTempBuffToPm(SourseAddr, DataSize, BufferType);
                        RdPmToTempBuff(SourseAddr, DataSize, BufferType);
                        SRbits.IPL = 0; //�������� ���������������� ����������

                        StepMotor1.State.InSavedPositionMarker = DevSetBuffer.Value.SM1_InSavedPositionMarker;
                    }
                    StepMotor1_On();
                }
            }
        }
    }
    if (Uart.Rx.Packet.Command == SM1_MOVE_BACK) {
        if (StepMotor1.State.ZeroSetMarker == YES) {
            if (StepMotor1.Trailer.State == OFF) {
                if (StepMotor1.State.GeneralStepCounter > 0) {
                    StartReadRxData();
                    ReadDataFromRxPacket(INT);
                    if(Uart.Rx.RxPackDataToRead.Int[0]>0){
                        if (Uart.Rx.RxPackDataToRead.Int[0] > StepMotor1.State.GeneralStepCounter) {
                            StepMotor1.MovingSettings.TargetPosition = 0;
                        } else {
                            StepMotor1.MovingSettings.TargetPosition = StepMotor1.State.GeneralStepCounter - (signed short int) Uart.Rx.RxPackDataToRead.Int[0];
                        }

                        StepMotor1.State.MovingStepCounter = 0;
                        StepMotor1.MovingSettings.Direction = BACK1;
                        StepMotor1.MovingSettings.MovingState = MOVING1;

                        if (StepMotor1.State.InSavedPositionMarker == YES) {
                            SourseAddr.Value = PM_DEV_SETTINGS_PAGE_ADDRESS;
                            DataSize = DATA_SIZE_DEV;
                            BufferType = DEV_SETTINGS;
                            SRbits.IPL = 7; //��������� ���������������� ����������
                            //������� ��������� ������������ �� ������ �������� � �����
                            RdPmToTempBuff(SourseAddr, DataSize, BufferType);
                            SRbits.IPL = 0; //�������� ���������������� ����������

                            DevSetBuffer.Value.SM1_InSavedPositionMarker = NO;

                            SRbits.IPL = 7; //��������� ���������������� ����������
                            ErasePage(SourseAddr.Words.HW, SourseAddr.Words.LW);
                            WrTempBuffToPm(SourseAddr, DataSize, BufferType);
                            RdPmToTempBuff(SourseAddr, DataSize, BufferType);
                            SRbits.IPL = 0; //�������� ���������������� ����������

                            StepMotor1.State.InSavedPositionMarker = DevSetBuffer.Value.SM1_InSavedPositionMarker;
                        }
                        StepMotor1_On();
                    }
                }
            }
        }
    }
    if (Uart.Rx.Packet.Command == SM1_STOP) {
        if(StepMotor1.State.DeterminatePositionMarker == NO){
            StepMotor1.MovingSettings.MovingState = STOP;
        }
    }

    if (Uart.Rx.Packet.Command == SM1_MOVE_TO_SAVED_POSITION) {
        if (StepMotor1.State.ZeroSetMarker == YES) {
            if (StepMotor1.State.SavedPosition != 0xFFFF) {
                if (StepMotor1.State.SavedPosition != StepMotor1.State.GeneralStepCounter) {
                    StepMotor1.MovingSettings.TargetPosition = StepMotor1.State.SavedPosition;
                    if (StepMotor1.MovingSettings.TargetPosition > StepMotor1.State.GeneralStepCounter) {
                        StepMotor1.MovingSettings.Direction = FORWARD1;
                    } else {
                        StepMotor1.MovingSettings.Direction = BACK1;
                    }
                    StepMotor1.State.MovingStepCounter = 0;
                    StepMotor1.MovingSettings.MovingState = MOVING1;
                    StepMotor1_On();
                }
            }
        }
    }
}

//_____________________________________________________________________________________
void SmTrailerReaction1(void) {//������� ��������� ��������� � ��������� ������� �� ��������� ��������� ����������  - CN ������ - ������ � CnReaction()
    switch (StepMotor1.Trailer.State) {
        case SETTING_ON://��������� �������� �� ��������� OFF � ��������� ON
            DisableCnInt();
            if (ZERO_TRAILER_PORT == 1) {//������� ��������� �N  => ������� ���������
                StepMotor1.Trailer.State = ON;
                EnableCnInt();
                if ((StepMotor1.MovingSettings.MovingState != STOP) && (StepMotor1.MovingSettings.Direction == BACK1)) {//�� ��������� � �������� � ����� ���������
                    StepMotor1.MovingSettings.MovingState = STOP;
                     while (T4CONbits.TON == 1) {//���� ���� ����� �������� ���
                    };
                    StepMotor1.MovingSettings.Direction = STOP;
                    StepMotor1.MovingSettings.TargetPosition = 0;

                    if (StepMotor1.State.ZeroSetMarker == NO) {
                        StepMotor1.State.ZeroSetMarker = YES;
                        StepMotor1.State.GeneralStepCounter = 0;
                    }
                    
                    if(StepMotor1.State.DeterminatePositionMarker == YES){
                        StepMotor1.MovingSettings.TargetPosition = StepMotor1.State.MovingStepCounter;
                        StepMotor1.State.MovingStepCounter = 0;
                        StepMotor1.MovingSettings.Direction = FORWARD1;
                        StepMotor1.MovingSettings.MovingState = MOVING1;                                    
                        StepMotor1_On();
                    }

                }else{
                    if (StepMotor1.State.ZeroSetMarker == NO) {
                        StepMotor1.State.ZeroSetMarker = YES;
                        StepMotor1.State.GeneralStepCounter = 0;
                    }
                }
            } else {//������� ��������� OFF  => ������� �� ��������� - �� ������
                StepMotor1.Trailer.State = OFF;
                EnableCnInt();
            }
        break;
        case SETTING_OFF://��������� �������� �� ��������� ON � ��������� OFF
            DisableCnInt();
            if (ZERO_TRAILER_PORT == 0) {//������� ��������� �FF  => ������� ���������
                StepMotor1.Trailer.State = OFF;
                EnableCnInt();
            } else {//������� ��������� ON  => ������� �� ��������� - �� ������
                StepMotor1.Trailer.State = ON;
                EnableCnInt();
            }
        break;
    }
}
