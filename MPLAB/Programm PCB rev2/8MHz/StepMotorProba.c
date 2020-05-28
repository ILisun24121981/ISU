
#include "StepMotorProba.h"
#include "US.h"

#include "uart.h"
#include "cn.h"
#include "packet.h"
#include "memory.h"
#include "PMTSensor.h"

volatile SM_t StepMotor;

//_____________________________________________________________________________________

void CongigPortsStepMotor(void) {//������� ��������� ������ ��
    // ��������� ������ ���������� ��������� ��
    // ������� A
    ENABLE_TRIS = 0;
    ENABLE_OD = 0;
    ENABLE_CNIE = 0;
    ENABLE_CNPUE = 0;
    ENABLE_CNPDE = 0;
    // ������� �
    RESET_TRIS = 0;
    RESET_OD = 0;
    RESET_CNIE = 0;
    RESET_CNPUE = 0;
    RESET_CNPDE = 0;
    // ������� �
    STEP_TRIS = 0;
    STEP_OD = 0;
    STEP_CNIE = 0;
    STEP_CNPUE = 0;
    STEP_CNPDE = 0;
    // ������� �
    DIR_TRIS = 0;
    DIR_OD = 0;
    DIR_CNIE = 0;
    DIR_CNPUE = 0;
    DIR_CNPDE = 0;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //��������� ������ ���������� ��
    // ������ �������� (�������� �������)
    END_TRAILER_TRIS = 1;
    END_TRAILER_OD = 0;
    END_TRAILER_CNIE = 1;
    END_TRAILER_CNPUE = 0;
    END_TRAILER_CNPDE = 0;
    // ������� �������� (��������� �������)
    START_TRAILER_TRIS = 1;
    START_TRAILER_OD = 0;
    START_TRAILER_CNIE = 1;
    START_TRAILER_CNPUE = 0;
    START_TRAILER_CNPDE = 0;
}//END CongigPortsStepMotor()
//_____________________________________________________________________________________

void InitStepMotor(void) { //������� ��������� �������������.
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // �������� ����� ��
    CongigPortsStepMotor();
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //������� ��������� ��������� ��
    //���������� ��������� DeveloperSettings, DefaultPresets , Presets ����������� �� ������ �������� � ������� InitPMSettings() ����� memory.�
    //��������� MovingSettings
    StepMotor.MovingSettings.MovingState = STOP;
    StepMotor.MovingSettings.Direction = STOP;
    StepMotor.MovingSettings.Speed = STOP;
    StepMotor.MovingSettings.TargetPosition = STOP;
    //��������� State
    StepMotor.State.GeneralStepCounter = 0;
    StepMotor.State.MovingStepCounter = 0;
    StepMotor.State.WorkMode = ADJUST;
    StepMotor.State.Block = ON;
    StepMotor.State.NeedStepReaction = NO;

    StepMotor.Trailer.DownBlock = ON;
    StepMotor.Trailer.UpBlock = ON;
    StepMotor.Trailer.Alarm = OFF;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // ������� ��������� ��������� ������ ���������� ��������� ��
    ENABLE_LAT = 1;
    RESET_LAT = 1;
    STEP_LAT = 0;
    DIR_LAT = 0;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // ��������� ��������� ��������� ����������
    // ������������ ����������� ����� �������, ��� ��� ��������� ��������� ��������� �� ����� ����� �������� 0 (=> 0-�������,1 - ���������)
    // ������ �������� ��������� �������! => �������� =  ��������� = 1 = ON (�� �������� = ������� = 0 = OFF)
    if (END_TRAILER_PORT == 0) {//������� = OFF
        StepMotor.Trailer.Down = SETTING_OFF;
    } else {//��������� = ON
        StepMotor.Trailer.Down = SETTING_ON;
    }
    // ������� �������� ��������� �������
    if (START_TRAILER_PORT == 0) {//������� = OFF
        StepMotor.Trailer.Up = SETTING_OFF;
    } else {//��������� = ON
        StepMotor.Trailer.Up = SETTING_ON;
    }
    SmTrailerReaction();
}//END InitStepMotor()
//_____________________________________________________________________________________

void CommandStepMotor(void) { //������� ��������� ������ �� c ��
    unsigned char i = 0;
    unsigned char direction;
    unsigned char movingstate;

    if (Uart.Rx.Packet.Command == SM_WRITE_DEV_SETTINGS) {//���������� ��������� ������������
        StepMotor.MovingSettings.MovingState = STOP;
        while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
        };
        StepMotor.State.NeedStepReaction = NO;
        //��������� ������

        SourseAddr.Value = PM_DEV_SETTINGS_PAGE_ADDRESS;
        DataSize = DATA_SIZE_DEV;
        BufferType = DEV_SETTINGS;
        SRbits.IPL = 7; //��������� ���������������� ����������
        //������� ��������� ������������ �� ������ �������� � �����
        RdPmToTempBuff(SourseAddr, DataSize, BufferType);
        SRbits.IPL = 0; //�������� ���������������� ����������

        //��������� �����
        StartReadRxData();
        ReadDataFromRxPacket(INT);
        DevSetBuffer.Value.SM_TransferCoefficientSL = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DevSetBuffer.Value.SM_MaxSpeed = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DevSetBuffer.Value.SM_MaxControlVolume = Uart.Rx.RxPackDataToRead.Int[0];
        //������ ������������� � �����
        if (DevSetBuffer.Value.SM_MaxSpeed == 0) {
            DevSetBuffer.Value.SM_MaxSpeed = DEFAULT_MIN_SPEED;
        }
        if (DevSetBuffer.Value.SM_MaxSpeed > DEFAULT_MIN_SPEED) {
            DevSetBuffer.Value.SM_MaxSpeed = DEFAULT_MIN_SPEED;
        }

        SRbits.IPL = 7; //��������� ���������������� ����������
        ErasePage(SourseAddr.Words.HW, SourseAddr.Words.LW);
        WrTempBuffToPm(SourseAddr, DataSize, BufferType);
        RdPmToTempBuff(SourseAddr, DataSize, BufferType);
        SRbits.IPL = 0; //�������� ���������������� ����������

        StepMotor.DeveloperSettings.TransferCoefficientSL = DevSetBuffer.Value.SM_TransferCoefficientSL;
        StepMotor.DeveloperSettings.MaxSpeed = DevSetBuffer.Value.SM_MaxSpeed;
        StepMotor.DeveloperSettings.MaxControlVolume = DevSetBuffer.Value.SM_MaxControlVolume;

        //�������� ������� ��������� � ������������ � ����� ���������� �������������
        if (StepMotor.Presets.ControlVolume > StepMotor.DeveloperSettings.MaxControlVolume) {
            StepMotor.Presets.ControlVolume = StepMotor.DeveloperSettings.MaxControlVolume;
        }
        if (StepMotor.Presets.MaxSpeedOil < StepMotor.DeveloperSettings.MaxSpeed) {
            StepMotor.Presets.MaxSpeedOil = StepMotor.DeveloperSettings.MaxSpeed;
        }

        i = 0;
        while (i <= FOURTH) {
            if (StepMotor.Presets.EndPhasePoint[i] < StepMotor.Presets.ControlVolume) {
                //TODO(1):a
                if (i == 0) {
                    if (StepMotor.Presets.PhaseSpeed[i] < StepMotor.DeveloperSettings.MaxSpeed) {
                        StepMotor.Presets.PhaseSpeed[i] = StepMotor.DeveloperSettings.MaxSpeed;
                    }
                } else {
                    if (StepMotor.Presets.PhaseSpeed[i] < StepMotor.Presets.MaxSpeedOil) {
                        StepMotor.Presets.PhaseSpeed[i] = StepMotor.Presets.MaxSpeedOil;
                    }
                }
                i++;
            } else {
                StepMotor.Presets.EndPhasePoint[i] = StepMotor.Presets.ControlVolume;
                if (i == 0) {
                    if (StepMotor.Presets.PhaseSpeed[i] < StepMotor.DeveloperSettings.MaxSpeed) {
                        StepMotor.Presets.PhaseSpeed[i] = StepMotor.DeveloperSettings.MaxSpeed;
                    }
                } else {
                    if (StepMotor.Presets.PhaseSpeed[i] < StepMotor.Presets.MaxSpeedOil) {
                        StepMotor.Presets.PhaseSpeed[i] = StepMotor.Presets.MaxSpeedOil;
                    }
                }
                i++;
                while (i <= FOURTH) {
                    StepMotor.Presets.EndPhasePoint[i] = StepMotor.Presets.ControlVolume;
                    StepMotor.Presets.PhaseSpeed[i] = DEFAULT_MIN_SPEED;
                    i++;
                }
            }
        }
        //SRbits.IPL = 0; //�������� ���������������� ����������
    }//!!!
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
    if (Uart.Rx.Packet.Command == SM_SET_PRESETS_ADDITIONAL) {//���������� �������������� ������� ���������
        StepMotor.MovingSettings.MovingState = STOP;
        while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
        };
        StepMotor.State.NeedStepReaction = NO;

        StartReadRxData();
        ReadDataFromRxPacket(INT);
        StepMotor.Presets.TransferCoefficientLV = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        StepMotor.Presets.MaxSpeedOil = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        StepMotor.Presets.ControlVolume = Uart.Rx.RxPackDataToRead.Int[0];

        if (StepMotor.Presets.MaxSpeedOil == 0) {
            StepMotor.Presets.MaxSpeedOil = DEFAULT_MIN_SPEED;
        }
        if (StepMotor.Presets.MaxSpeedOil > DEFAULT_MIN_SPEED) {
            StepMotor.Presets.MaxSpeedOil = DEFAULT_MIN_SPEED;
        }
        if (StepMotor.Presets.MaxSpeedOil < StepMotor.DeveloperSettings.MaxSpeed) {
            StepMotor.Presets.MaxSpeedOil = StepMotor.DeveloperSettings.MaxSpeed;
        }
        if (StepMotor.Presets.ControlVolume > StepMotor.DeveloperSettings.MaxControlVolume) {
            StepMotor.Presets.ControlVolume = StepMotor.DeveloperSettings.MaxControlVolume;
        }

        //������������� ������� ��������� ��� ������� � ������������ � ����� ���������� �������������
        i = 0;
        while (i <= FOURTH) {
            if (StepMotor.Presets.EndPhasePoint[i] < StepMotor.Presets.ControlVolume) {
                if (i != 0) {
                    if (StepMotor.Presets.PhaseSpeed[i] < StepMotor.Presets.MaxSpeedOil) {
                        StepMotor.Presets.PhaseSpeed[i] = StepMotor.Presets.MaxSpeedOil;
                    }
                }
            } else {
                StepMotor.Presets.EndPhasePoint[i] = StepMotor.Presets.ControlVolume;
                if (i != 0) {
                    if (StepMotor.Presets.PhaseSpeed[i] < StepMotor.Presets.MaxSpeedOil) {
                        StepMotor.Presets.PhaseSpeed[i] = StepMotor.Presets.MaxSpeedOil;
                    }
                }
                i++;
                while (i <= FOURTH) {
                    StepMotor.Presets.EndPhasePoint[i] = StepMotor.Presets.ControlVolume;
                    StepMotor.Presets.PhaseSpeed[i] = DEFAULT_MIN_SPEED;
                    i++;
                }
            }
            i++;
        }
    }//!!!
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SM_SET_PRESETS_PHASE) {//���������� ������� ��������� ��� �������
        StepMotor.MovingSettings.MovingState = STOP;
        while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
        };
        StepMotor.State.NeedStepReaction = NO;

        StartReadRxData();
        ReadDataFromRxPacket(INT);
        i = 0;
        while (i <= FOURTH) {
            if (Uart.Rx.RxPackDataToRead.Int[0] < StepMotor.Presets.ControlVolume) {
                if (i == 0) {
                    StepMotor.Presets.EndPhasePoint[i] = Uart.Rx.RxPackDataToRead.Int[0];
                } else {
                    if ((StepMotor.Presets.ControlVolume - StepMotor.Presets.EndPhasePoint[i - 1]) < Uart.Rx.RxPackDataToRead.Int[0]) {
                        StepMotor.Presets.EndPhasePoint[i] = StepMotor.Presets.ControlVolume;
                    } else {
                        StepMotor.Presets.EndPhasePoint[i] = StepMotor.Presets.EndPhasePoint[i - 1] + Uart.Rx.RxPackDataToRead.Int[0];
                    }
                }
            } else {
                StepMotor.Presets.EndPhasePoint[i] = StepMotor.Presets.ControlVolume;
            }
            if (i > 0) {
                if (StepMotor.Presets.EndPhasePoint[i] < StepMotor.Presets.EndPhasePoint[i - 1]) {//�� ���������� ������� ��� ���� - ������
                    StepMotor.Presets.EndPhasePoint[i] = StepMotor.Presets.EndPhasePoint[i - 1];
                }
            }
            ReadDataFromRxPacket(INT);
            if (i == 0) {
                if (Uart.Rx.RxPackDataToRead.Int[0] < StepMotor.DeveloperSettings.MaxSpeed) {
                    StepMotor.Presets.PhaseSpeed[i] = StepMotor.DeveloperSettings.MaxSpeed;
                }else{
                    if (Uart.Rx.RxPackDataToRead.Int[0] > DEFAULT_MIN_SPEED) {
                        Uart.Rx.RxPackDataToRead.Int[0] = DEFAULT_MIN_SPEED;
                    }
                    StepMotor.Presets.PhaseSpeed[i] = Uart.Rx.RxPackDataToRead.Int[0];
                }
            } else {
                if (Uart.Rx.RxPackDataToRead.Int[0] < StepMotor.Presets.MaxSpeedOil) {
                    StepMotor.Presets.PhaseSpeed[i] = StepMotor.Presets.MaxSpeedOil;
                } else {
                    if (Uart.Rx.RxPackDataToRead.Int[0] > DEFAULT_MIN_SPEED) {
                        Uart.Rx.RxPackDataToRead.Int[0] = DEFAULT_MIN_SPEED;
                    }
                    StepMotor.Presets.PhaseSpeed[i] = Uart.Rx.RxPackDataToRead.Int[0];
                }
            }

//            if (Uart.Rx.RxPackDataToRead.Int[0] < StepMotor.Presets.MaxSpeedOil) {
//                //TODO(1):b
//                if (i == 0) {
//                    if (Uart.Rx.RxPackDataToRead.Int[0] < StepMotor.DeveloperSettings.MaxSpeed) {
//                        StepMotor.Presets.PhaseSpeed[i] = StepMotor.DeveloperSettings.MaxSpeed;
//                    } else {
//                        StepMotor.Presets.PhaseSpeed[i] = Uart.Rx.RxPackDataToRead.Int[0];
//                    }
//                } else {//i!= 0
//                    StepMotor.Presets.PhaseSpeed[i] = StepMotor.Presets.MaxSpeedOil;
//
//                }
//            } else {
//                StepMotor.Presets.PhaseSpeed[i] = Uart.Rx.RxPackDataToRead.Int[0];
//                if (StepMotor.Presets.PhaseSpeed[i] > DEFAULT_MIN_SPEED) {
//                    StepMotor.Presets.PhaseSpeed[i] = DEFAULT_MIN_SPEED;
//                }
//            }
            if (StepMotor.Presets.EndPhasePoint[i] == StepMotor.Presets.ControlVolume) {
                i++;
                while (i <= FOURTH) {
                    StepMotor.Presets.EndPhasePoint[i] = StepMotor.Presets.ControlVolume;
                    StepMotor.Presets.PhaseSpeed[i] = DEFAULT_MIN_SPEED;
                    i++;
                }
            } else {
                ReadDataFromRxPacket(INT);
                i++;
            }
        }
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SM_WRITE_PRESETS_AS_DEFAULT) {//�������� ������� ��������� � ��������� �� ���������
        StepMotor.MovingSettings.MovingState = STOP;
        while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
        };
        StepMotor.State.NeedStepReaction = NO;

        SourseAddr.Value = PM_DEF_SETTINGS_PAGE_ADDRESS;
        DataSize = DATA_SIZE_DEF;
        BufferType = DEF_SETTINGS;
        SRbits.IPL = 7; //��������� ���������������� ����������
        //������� ��������� �� ��������� �� ������ �������� � �����
        RdPmToTempBuff(SourseAddr, DataSize, BufferType);
        SRbits.IPL = 0; //�������� ���������������� ����������

        //�������� �������������� ��������� �� ���������
        DefSetBuffer.Value.SM_MaxSpeedOil = StepMotor.Presets.MaxSpeedOil;
        DefSetBuffer.Value.SM_TransferCoefficientLV = StepMotor.Presets.TransferCoefficientLV;
        DefSetBuffer.Value.SM_ControlVolume = StepMotor.Presets.ControlVolume;

        i = 0;
        while (i <= FOURTH) {
            DefSetBuffer.Value.SM_EndPhasePoint[i] = StepMotor.Presets.EndPhasePoint[i];
            DefSetBuffer.Value.SM_PhaseSpeed[i] = StepMotor.Presets.PhaseSpeed[i];
            i++;
        }

        SRbits.IPL = 7; //��������� ���������������� ����������
        ErasePage(SourseAddr.Words.HW, SourseAddr.Words.LW);
        WrTempBuffToPm(SourseAddr, DataSize, BufferType);
        RdPmToTempBuff(SourseAddr, DataSize, BufferType);
        SRbits.IPL = 0; //�������� ���������������� ����������

        StepMotor.DefaultPresets.MaxSpeedOil = DefSetBuffer.Value.SM_MaxSpeedOil;
        StepMotor.DefaultPresets.TransferCoefficientLV = DefSetBuffer.Value.SM_TransferCoefficientLV;
        StepMotor.DefaultPresets.ControlVolume = DefSetBuffer.Value.SM_ControlVolume;

        i = 0;
        while (i <= FOURTH) {
            StepMotor.DefaultPresets.EndPhasePoint[i] = DefSetBuffer.Value.SM_EndPhasePoint[i];
            StepMotor.DefaultPresets.PhaseSpeed[i] = DefSetBuffer.Value.SM_PhaseSpeed[i];
            i++;
        }

        StepMotor.Presets.MaxSpeedOil = StepMotor.DefaultPresets.MaxSpeedOil;
        StepMotor.Presets.TransferCoefficientLV = StepMotor.DefaultPresets.TransferCoefficientLV;
        StepMotor.Presets.ControlVolume = StepMotor.DefaultPresets.ControlVolume;

        i = 0;
        while (i <= FOURTH) {
            StepMotor.Presets.EndPhasePoint[i] = StepMotor.DefaultPresets.EndPhasePoint[i];
            StepMotor.Presets.PhaseSpeed[i] = StepMotor.DefaultPresets.PhaseSpeed[i];
            i++;
        }

        //SRbits.IPL = 0; //�������� ���������������� ����������
    }//!!!
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SM_LOAD_DEFAULT_TO_PRESETS) {//�������� ��������� �� ��������� ��� ��� �������
        StepMotor.MovingSettings.MovingState = STOP;
        while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
        };
        StepMotor.State.NeedStepReaction = NO;

        //�������� �������������� ������� ���������
        StepMotor.Presets.MaxSpeedOil = StepMotor.DefaultPresets.MaxSpeedOil;
        StepMotor.Presets.TransferCoefficientLV = StepMotor.DefaultPresets.TransferCoefficientLV;
        StepMotor.Presets.ControlVolume = StepMotor.DefaultPresets.ControlVolume;
        //�������� ������� ��������� ���
        i = 0;
        while (i <= FOURTH) {
            StepMotor.Presets.EndPhasePoint[i] = StepMotor.DefaultPresets.EndPhasePoint[i];
            StepMotor.Presets.PhaseSpeed[i] = StepMotor.DefaultPresets.PhaseSpeed[i];
            i++;
        }

        //�������� ����������� �������� �������������� ������� �������� �� ������ ���������� ���������� ��������
        if (StepMotor.Presets.ControlVolume > StepMotor.DeveloperSettings.MaxControlVolume) {
            StepMotor.Presets.ControlVolume = StepMotor.DeveloperSettings.MaxControlVolume;
        }
        if (StepMotor.Presets.MaxSpeedOil < StepMotor.DeveloperSettings.MaxSpeed) {
            StepMotor.Presets.MaxSpeedOil = StepMotor.DeveloperSettings.MaxSpeed;
        }

        //�������� ����������� �������� ������x �������� ��� � ������������ � ������������� ���������� �������������� ������� ��������
        i = 0;
        while (i <= FOURTH) {
            if (StepMotor.Presets.EndPhasePoint[i] < StepMotor.Presets.ControlVolume) {
                //TODO(1):a
                if (i == 0) {
                    if (StepMotor.Presets.PhaseSpeed[i] < StepMotor.DeveloperSettings.MaxSpeed) {
                        StepMotor.Presets.PhaseSpeed[i] = StepMotor.DeveloperSettings.MaxSpeed;
                    }
                } else {
                    if (StepMotor.Presets.PhaseSpeed[i] < StepMotor.Presets.MaxSpeedOil) {
                        StepMotor.Presets.PhaseSpeed[i] = StepMotor.Presets.MaxSpeedOil;
                    }
                }
                i++;
            } else {
                StepMotor.Presets.EndPhasePoint[i] = StepMotor.Presets.ControlVolume;
                if (i == 0) {
                    if (StepMotor.Presets.PhaseSpeed[i] < StepMotor.DeveloperSettings.MaxSpeed) {
                        StepMotor.Presets.PhaseSpeed[i] = StepMotor.DeveloperSettings.MaxSpeed;
                    }
                } else {
                    if (StepMotor.Presets.PhaseSpeed[i] < StepMotor.Presets.MaxSpeedOil) {
                        StepMotor.Presets.PhaseSpeed[i] = StepMotor.Presets.MaxSpeedOil;
                    }
                }
                i++;
                while (i <= FOURTH) {
                    StepMotor.Presets.EndPhasePoint[i] = StepMotor.Presets.ControlVolume;
                    StepMotor.Presets.PhaseSpeed[i] = DEFAULT_MIN_SPEED;
                    i++;
                }
            }
        }
//        if (StepMotor.Presets.ControlVolume > StepMotor.DeveloperSettings.MaxControlVolume) {
//            StepMotor.Presets.ControlVolume = StepMotor.DeveloperSettings.MaxControlVolume;
//        }
//        if (StepMotor.Presets.MaxSpeedOil < StepMotor.DeveloperSettings.MaxSpeed) {
//            StepMotor.Presets.MaxSpeedOil = StepMotor.DeveloperSettings.MaxSpeed;
//        }
//
//        i = 0;
//        while (i <= FOURTH) {
//            if (StepMotor.Presets.EndPhasePoint[i] < StepMotor.Presets.ControlVolume) {
//                if (i == 0) {
//                    if (StepMotor.Presets.PhaseSpeed[i] < StepMotor.DeveloperSettings.MaxSpeed) {
//                        StepMotor.Presets.PhaseSpeed[i] = StepMotor.DeveloperSettings.MaxSpeed;
//                    }
//                } else {
//                    if (StepMotor.Presets.PhaseSpeed[i] < StepMotor.Presets.MaxSpeedOil) {
//                        StepMotor.Presets.PhaseSpeed[i] = StepMotor.Presets.MaxSpeedOil;
//                    }
//                }
//                i++;
//            } else {
//                StepMotor.Presets.EndPhasePoint[i] = StepMotor.Presets.ControlVolume;
//                if (i == 0) {
//                    if (StepMotor.Presets.PhaseSpeed[i] < StepMotor.DeveloperSettings.MaxSpeed) {
//                        StepMotor.Presets.PhaseSpeed[i] = StepMotor.DeveloperSettings.MaxSpeed;
//                    }
//                } else {
//                    if (StepMotor.Presets.PhaseSpeed[i] < StepMotor.Presets.MaxSpeedOil) {
//                        StepMotor.Presets.PhaseSpeed[i] = StepMotor.Presets.MaxSpeedOil;
//                    }
//                }
//                i++;
//                while (i <= FOURTH) {
//                    StepMotor.Presets.EndPhasePoint[i] = StepMotor.Presets.ControlVolume;
//                    StepMotor.Presets.PhaseSpeed[i] = DEFAULT_MIN_SPEED;
//                    i++;
//                }
//            }
//        }

    }

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SM_SET_BLOCK) {//���������� ��������� �������
        StepMotor.MovingSettings.MovingState = STOP;
        while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
        };
        StepMotor.State.NeedStepReaction = NO;
        if (StepMotor.State.Block == ON) {
            StepMotor.State.Block = OFF;
        } else {
            StepMotor.State.Block = ON;
        }
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SM_SET_UP_TRAILER_BLOCK) {//���������� ��������� �������
        StepMotor.MovingSettings.MovingState = STOP;
        while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
        };
        StepMotor.State.NeedStepReaction = NO;
        if (StepMotor.Trailer.UpBlock == ON) {
            StepMotor.Trailer.UpBlock = OFF;
        } else {
            StepMotor.Trailer.UpBlock = ON;
        }
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SM_SET_DOWN_TRAILER_BLOCK) {//���������� ��������� �������
        StepMotor.MovingSettings.MovingState = STOP;
        while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
        };
        StepMotor.State.NeedStepReaction = NO;
        if (StepMotor.Trailer.DownBlock == ON) {
            StepMotor.Trailer.DownBlock = OFF;
        } else {
            StepMotor.Trailer.DownBlock = ON;
        }
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SM_SET_ZERO) {//���������� ��������� �������
        if (StepMotor.State.WorkMode == ADJUST) {
            StepMotor.MovingSettings.MovingState = STOP;
            while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
            };
            StepMotor.State.NeedStepReaction = NO;
            StepMotor.State.GeneralStepCounter = 0;
        }
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SM_SET_WORK_MODE) {//������ ������� �����
        StepMotor.MovingSettings.MovingState = STOP;
        while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
        };
        if (StepMotor.State.WorkMode == ADJUST) {
            StepMotor.State.NeedStepReaction = NO;
            StepMotor.State.WorkMode = STANDART;
            StepMotor.State.Zero = YES;
        } else {
            StepMotor.State.NeedStepReaction = NO;
            StepMotor.State.WorkMode = ADJUST;
            StepMotor.State.Zero = NO;
        }

    }
    if (Uart.Rx.Packet.Command == SM_COLIBRATE_DONE) {//������ ������� �����
        if (StepMotor.State.WorkMode == ADJUST) {
            StepMotor.MovingSettings.MovingState = STOP;
            while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
            };
            StepMotor.State.NeedStepReaction = NO;
            StepMotor.State.GeneralStepCounter = 0;
            StepMotor.State.WorkMode = STANDART;
            StepMotor.State.Zero = YES;
        }
    }
    if (Uart.Rx.Packet.Command == SM_COLIBRATE_NOT_DONE) {//������ ����� ���������
        StepMotor.MovingSettings.MovingState = STOP;
        while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
        };
        if (StepMotor.State.WorkMode == STANDART) {
            StepMotor.State.NeedStepReaction = NO;
            StepMotor.State.WorkMode = ADJUST;
            StepMotor.State.Zero = NO;
        }
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SM_STOP) {// ���������� ��
        if (StepMotor.MovingSettings.MovingState != STOP) {//� ��������
            direction = StepMotor.MovingSettings.Direction; //��� ����)!!! ��� ��� � ���������� �� ������� 2 ��������� ����������� � ���������� ���������
            movingstate = StepMotor.MovingSettings.MovingState;
            StepMotor.MovingSettings.MovingState = STOP;
            while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
            };
            StepMotor.State.NeedStepReaction = NO;
            if (StepMotor.State.WorkMode == STANDART) {
                if(movingstate == PHASES_MOVING){
                    if ((StepMotor.State.GeneralStepCounter >= StepMotor.Presets.EndPhasePoint[FIRST]) && (direction == FORWARD)) {
                        StepMotor.MovingSettings.Direction = BACK;
                        StepMotor.MovingSettings.MovingState = SIMPLE_MOVING;
                        StepMotor.MovingSettings.Speed = StepMotor.DeveloperSettings.MaxSpeed;
                        PR2 = StepMotor.MovingSettings.Speed;
                        StepMotor.MovingSettings.TargetPosition = 0;
                        StepMotor.State.MovingStepCounter = 0;
                        StepMotor_On();
                    }
                }
            }
        }
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SM_RETURN) {//������� �������� � ���������/� ��������� �������(� ����������� �� ������)
        StepMotor.MovingSettings.MovingState = STOP;
        while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
        };
        StepMotor.State.NeedStepReaction = NO;
        if ((StepMotor.Trailer.Up != ON) || (StepMotor.Trailer.UpBlock == OFF)) {
            if (StepMotor.State.WorkMode == STANDART) {
                if (StepMotor.State.GeneralStepCounter > 0) {//��������� �� � ��������� �������
                    StepMotor.MovingSettings.Direction = BACK;
                    StepMotor.MovingSettings.MovingState = SIMPLE_MOVING;
                    StepMotor.MovingSettings.Speed = StepMotor.DeveloperSettings.MaxSpeed;
                    PR2 = StepMotor.MovingSettings.Speed;
                    StepMotor.MovingSettings.TargetPosition = 0;
                    StepMotor.State.MovingStepCounter = 0;
                    StepMotor_On();
                }
            } else {//StepMotor.State.WorkMode == ADJUST
                //if (StepMotor.Trailer.Up != ON) {
                StepMotor.MovingSettings.Direction = BACK;
                StepMotor.MovingSettings.MovingState = SIMPLE_MOVING;
                StepMotor.MovingSettings.Speed = StepMotor.DeveloperSettings.MaxSpeed;
                PR2 = StepMotor.MovingSettings.Speed;
                StepMotor.MovingSettings.TargetPosition = StepMotor.State.GeneralStepCounter - (signed long) StepMotor.Presets.ControlVolume;
                StepMotor.State.MovingStepCounter = 0;
                StepMotor_On();
                //}
            }
        }else{
            StepMotor.State.WorkMode = STANDART;
            StepMotor.State.GeneralStepCounter = 0;
            StepMotor.State.Zero = YES;
        }
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SM_START_PHASES) {//������� ������� �������� � ������������ � ���������� �������
        StepMotor.MovingSettings.MovingState = STOP;
        while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
        };
        StepMotor.State.NeedStepReaction = NO;
        if((StepMotor.Trailer.Down == OFF)||(StepMotor.Trailer.DownBlock == OFF)){
            if ((PMTSensor.State == ON) || (StepMotor.State.Block == OFF)) {
                if (StepMotor.State.GeneralStepCounter >= 0) {
                    if (StepMotor.State.GeneralStepCounter < (signed long) StepMotor.Presets.EndPhasePoint[FOURTH]) {
                        if (StepMotor.State.GeneralStepCounter >= (signed long) StepMotor.Presets.EndPhasePoint[THIRD]) {
                            StepMotor.MovingSettings.Speed = StepMotor.Presets.PhaseSpeed[FOURTH];
                            StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.Presets.EndPhasePoint[FOURTH];
                        } else {
                            if (StepMotor.State.GeneralStepCounter >= (signed long) StepMotor.Presets.EndPhasePoint[SECOND]) {
                                StepMotor.MovingSettings.Speed = StepMotor.Presets.PhaseSpeed[THIRD];
                                StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.Presets.EndPhasePoint[THIRD];
                            } else {
                                if (StepMotor.State.GeneralStepCounter >= (signed long) StepMotor.Presets.EndPhasePoint[FIRST]) {
                                    StepMotor.MovingSettings.Speed = StepMotor.Presets.PhaseSpeed[SECOND];
                                    StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.Presets.EndPhasePoint[SECOND];
                                } else {
                                    if (StepMotor.State.GeneralStepCounter >= (signed long) StepMotor.Presets.EndPhasePoint[ZERO]) {
                                        StepMotor.MovingSettings.Speed = StepMotor.Presets.PhaseSpeed[FIRST];
                                        StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.Presets.EndPhasePoint[FIRST];
                                    } else {
                                        StepMotor.MovingSettings.Speed = StepMotor.Presets.PhaseSpeed[ZERO];
                                        StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.Presets.EndPhasePoint[ZERO];
                                    }
                                }
                            }
                        }
                        PR2 = StepMotor.MovingSettings.Speed;
                        StepMotor.MovingSettings.Direction = FORWARD;
                        StepMotor.MovingSettings.MovingState = PHASES_MOVING;
                        StepMotor.State.MovingStepCounter = 0;
                        Reason = NO;
                        StepMotor_On();
                        US_On();
                    }
                }
            }
        }
    }

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SM_MOVE_BACK) {
        StepMotor.MovingSettings.MovingState = STOP;
        while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
        };
        StepMotor.State.NeedStepReaction = NO;
        if ((StepMotor.Trailer.Up != ON) || (StepMotor.Trailer.UpBlock == OFF)) {
            StartReadRxData();
            ReadDataFromRxPacket(INT);
            if (Uart.Rx.RxPackDataToRead.Int[0] != 0) {
                if (Uart.Rx.RxPackDataToRead.Int[0] >= StepMotor.Presets.ControlVolume) {
                    Uart.Rx.RxPackDataToRead.Int[0] = StepMotor.Presets.ControlVolume;
                }
                switch (StepMotor.State.WorkMode) {
                    case STANDART:
                        if (StepMotor.State.GeneralStepCounter > 0) {
                            StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.State.GeneralStepCounter - (signed long) Uart.Rx.RxPackDataToRead.Int[0];
                            if (StepMotor.MovingSettings.TargetPosition < 0) {
                                StepMotor.MovingSettings.TargetPosition = 0;
                            }
                            ReadDataFromRxPacket(INT);
                            StepMotor.MovingSettings.Speed = Uart.Rx.RxPackDataToRead.Int[0];
                            if (StepMotor.MovingSettings.Speed < StepMotor.DeveloperSettings.MaxSpeed) {
                                StepMotor.MovingSettings.Speed = StepMotor.DeveloperSettings.MaxSpeed;
                            }
                            PR2 = StepMotor.MovingSettings.Speed;
                            StepMotor.State.MovingStepCounter = 0;
                            StepMotor.MovingSettings.Direction = BACK;
                            StepMotor.MovingSettings.MovingState = SIMPLE_MOVING;
                            StepMotor_On();
                        }
                        break;
                    case ADJUST:
                        if (StepMotor.State.GeneralStepCounter > (-(signed long) StepMotor.Presets.ControlVolume)) {
                            StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.State.GeneralStepCounter - (signed long) Uart.Rx.RxPackDataToRead.Int[0];
                            if (StepMotor.MovingSettings.TargetPosition < (-(signed long) StepMotor.Presets.ControlVolume)) {
                                StepMotor.MovingSettings.TargetPosition = -(signed long) StepMotor.Presets.ControlVolume;
                            }
                            ReadDataFromRxPacket(INT);
                            StepMotor.MovingSettings.Speed = Uart.Rx.RxPackDataToRead.Int[0];
                            if (StepMotor.MovingSettings.Speed < StepMotor.DeveloperSettings.MaxSpeed) {
                                StepMotor.MovingSettings.Speed = StepMotor.DeveloperSettings.MaxSpeed;
                            }
                            PR2 = StepMotor.MovingSettings.Speed;
                            StepMotor.State.MovingStepCounter = 0;
                            StepMotor.MovingSettings.Direction = BACK;
                            StepMotor.MovingSettings.MovingState = SIMPLE_MOVING;
                            StepMotor_On();
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SM_MOVE_FORWARD) {
        movingstate = StepMotor.MovingSettings.MovingState;
        StepMotor.MovingSettings.MovingState = STOP;
        while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
        };
        StepMotor.State.NeedStepReaction = NO;
        if ((PMTSensor.State == ON) || (StepMotor.State.Block == OFF)) {
            if ((StepMotor.Trailer.Down == OFF) || (StepMotor.Trailer.DownBlock == OFF)) {
                StartReadRxData();
                ReadDataFromRxPacket(INT);
                if (Uart.Rx.RxPackDataToRead.Int[0] != 0) {
                    if (Uart.Rx.RxPackDataToRead.Int[0] > StepMotor.Presets.ControlVolume) {
                        Uart.Rx.RxPackDataToRead.Int[0] = StepMotor.Presets.ControlVolume;
                    }
                    switch (StepMotor.State.WorkMode) {
                        case STANDART:
                            if (movingstate != PHASES_MOVING) {//��� ��������� � ������ ���� �������� ���� �� ����� - �������)))) ������� ����� �� ������, ���� ��� ����� ��� ���!!! - ���� ������)
                                if (StepMotor.State.GeneralStepCounter < StepMotor.Presets.EndPhasePoint[FOURTH]) {
                                    StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.State.GeneralStepCounter + (signed long) Uart.Rx.RxPackDataToRead.Int[0];
                                    if (StepMotor.MovingSettings.TargetPosition > StepMotor.Presets.EndPhasePoint[FOURTH]) {
                                        StepMotor.MovingSettings.TargetPosition = StepMotor.Presets.EndPhasePoint[FOURTH];
                                    }
                                    ReadDataFromRxPacket(INT);
                                    StepMotor.MovingSettings.Speed = Uart.Rx.RxPackDataToRead.Int[0];
                                    if (StepMotor.MovingSettings.Speed < StepMotor.DeveloperSettings.MaxSpeed) {
                                        StepMotor.MovingSettings.Speed = StepMotor.DeveloperSettings.MaxSpeed;
                                    }
                                    PR2 = StepMotor.MovingSettings.Speed;
                                    StepMotor.State.MovingStepCounter = 0;
                                    StepMotor.MovingSettings.Direction = FORWARD;
                                    StepMotor.MovingSettings.MovingState = SIMPLE_MOVING;
                                    StepMotor_On();
                                }
                            }
                            break;
                        case ADJUST:
                            if (StepMotor.State.GeneralStepCounter < (signed long) StepMotor.Presets.ControlVolume) {
                                StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.State.GeneralStepCounter + (signed long) Uart.Rx.RxPackDataToRead.Int[0];
                                if (StepMotor.MovingSettings.TargetPosition > (signed long) StepMotor.Presets.ControlVolume) {
                                    StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.Presets.ControlVolume;
                                }
                                ReadDataFromRxPacket(INT);
                                StepMotor.MovingSettings.Speed = Uart.Rx.RxPackDataToRead.Int[0];
                                if (StepMotor.MovingSettings.Speed < StepMotor.DeveloperSettings.MaxSpeed) {
                                    StepMotor.MovingSettings.Speed = StepMotor.DeveloperSettings.MaxSpeed;
                                }
                                PR2 = StepMotor.MovingSettings.Speed;
                                StepMotor.State.MovingStepCounter = 0;
                                StepMotor.MovingSettings.Direction = FORWARD;
                                StepMotor.MovingSettings.MovingState = SIMPLE_MOVING;
                                StepMotor_On();
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    if (Uart.Rx.Packet.Command == SM_WASHING) {
        StepMotor.MovingSettings.MovingState = STOP;
        while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
        };
        StepMotor.State.NeedStepReaction = NO;
        if ((PMTSensor.State == ON) || (StepMotor.State.Block == OFF)) {
            if ((StepMotor.Trailer.Down == OFF) || (StepMotor.Trailer.DownBlock == OFF)) {
                switch (StepMotor.State.WorkMode) {
                    case STANDART:
                        if (StepMotor.State.GeneralStepCounter < StepMotor.Presets.EndPhasePoint[FOURTH]) {
                            StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.Presets.EndPhasePoint[FOURTH];
                            StepMotor.MovingSettings.Speed = StepMotor.Presets.MaxSpeedOil;
                            if (StepMotor.State.GeneralStepCounter < StepMotor.Presets.EndPhasePoint[ZERO]) {
                                StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.Presets.EndPhasePoint[ZERO];
                                StepMotor.MovingSettings.Speed = StepMotor.Presets.PhaseSpeed[ZERO];
                            }
                            PR2 = StepMotor.MovingSettings.Speed;
                            StepMotor.State.MovingStepCounter = 0;
                            StepMotor.MovingSettings.Direction = FORWARD;
                            StepMotor.MovingSettings.MovingState = WASHING_MOVING;
                            Reason = NO;
                            StepMotor_On();
                            US_On();
                        }
                        break;
                    case ADJUST:

                         if (StepMotor.State.GeneralStepCounter < StepMotor.Presets.EndPhasePoint[ZERO]) {
                            StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.Presets.EndPhasePoint[ZERO];
                            StepMotor.MovingSettings.Speed = StepMotor.Presets.PhaseSpeed[ZERO];
                        }else{
                            if (StepMotor.State.GeneralStepCounter < StepMotor.Presets.EndPhasePoint[FOURTH]) {
                                StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.Presets.EndPhasePoint[FOURTH];
                            }else{
                                StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.Presets.ControlVolume;
                            }
                            StepMotor.MovingSettings.Speed = StepMotor.Presets.MaxSpeedOil;
                        }
                        PR2 = StepMotor.MovingSettings.Speed;
                        StepMotor.State.MovingStepCounter = 0;
                        StepMotor.MovingSettings.Direction = FORWARD;
                        StepMotor.MovingSettings.MovingState = WASHING_MOVING;
                        Reason = NO;
                        StepMotor_On();
                        US_On();
//                        if (StepMotor.State.GeneralStepCounter < (signed long) StepMotor.Presets.ControlVolume) {
//                            StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.Presets.ControlVolume;
//                            StepMotor.MovingSettings.Speed = StepMotor.Presets.MaxSpeedOil;
//                            PR2 = StepMotor.MovingSettings.Speed;
//                            StepMotor.State.MovingStepCounter = 0;
//                            StepMotor.MovingSettings.Direction = FORWARD;
//                            StepMotor.MovingSettings.Mode = SIMPLE_MODE;
//                            StepMotor_On();
//                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
}//END CommandStepMotor()
//_____________________________________________________________________________________

void SmTrailerReaction(void) {//������� ��������� ��������� � ��������� ������� �� ��������� ��������� ����������  - CN ������ - ������ � CnReaction()

    switch (StepMotor.Trailer.Down) {
        case SETTING_ON://��������� �������� �� ��������� OFF � ��������� ON.
            DisableCnInt();
            if (END_TRAILER_PORT == 1) {//������� ��������� �N => ������� ���������
                StepMotor.Trailer.Down = ON;
                EnableCnInt();
                if (StepMotor.Trailer.DownBlock == ON) {
                    if ((StepMotor.MovingSettings.MovingState != STOP) && (StepMotor.MovingSettings.Direction == FORWARD)) {//�� ��������� � �������� � ����� ���������
                        StepMotor.MovingSettings.MovingState = STOP;
                        while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
                        };
                        if (StepMotor.State.WorkMode == STANDART) {
                            if (StepMotor.State.GeneralStepCounter >= StepMotor.Presets.EndPhasePoint[FIRST]) {
                                StepMotor.MovingSettings.Direction = BACK;
                                StepMotor.MovingSettings.MovingState = SIMPLE_MOVING;
                                StepMotor.MovingSettings.Speed = StepMotor.DeveloperSettings.MaxSpeed;
                                PR2 = StepMotor.MovingSettings.Speed;
                                StepMotor.MovingSettings.TargetPosition = 0;
                                StepMotor.State.MovingStepCounter = 0;
                                StepMotor_On();
                            }
                        }
                    }
                }
            } else {//������� ��������� OFF  => ������� �� ��������� - �� ������
                StepMotor.Trailer.Down = OFF;
                EnableCnInt();
            }
            break;
        case SETTING_OFF://��������� �������� �� ��������� ON � ��������� OFF
            DisableCnInt();
            if (END_TRAILER_PORT == 0) {//������� ��������� �FF  => ������� ���������
                StepMotor.Trailer.Down = OFF;
                EnableCnInt();
            } else {//������� ��������� ON  => ������� �� ��������� - �� ������
                StepMotor.Trailer.Down = ON;
                EnableCnInt();
            }
            break;
    }
    switch (StepMotor.Trailer.Up) {
        case SETTING_ON://��������� �������� �� ��������� OFF � ��������� ON
            DisableCnInt();
            if (START_TRAILER_PORT == 1) {//������� ��������� �N  => ������� ���������
                StepMotor.Trailer.Up = ON;
                EnableCnInt();
                if (StepMotor.Trailer.UpBlock == ON) {
                    if (StepMotor.MovingSettings.MovingState != STOP) {
                        if (StepMotor.MovingSettings.Direction == BACK) {//�� ��������� � �������� � ����� ���������
                            StepMotor.MovingSettings.MovingState = STOP;
                            while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
                            };
                            US_Off();
                            if (StepMotor.State.WorkMode != STANDART) {
                                StepMotor.State.WorkMode = STANDART;
                                StepMotor.State.GeneralStepCounter = 0;
                                StepMotor.State.Zero = YES;
                            }
                        }
                    } else {
                        if (StepMotor.State.WorkMode != STANDART) {
                            StepMotor.State.WorkMode = STANDART;
                            StepMotor.State.GeneralStepCounter = 0;
                            StepMotor.State.Zero = YES;
                        }
                    }
                }
            } else {//������� ��������� OFF  => ������� �� ��������� - �� ������
                StepMotor.Trailer.Up = OFF;
                EnableCnInt();
            }
            break;
        case SETTING_OFF://��������� �������� �� ��������� ON � ��������� OFF
            DisableCnInt();
            if (START_TRAILER_PORT == 0) {//������� ��������� �FF  => ������� ���������
                StepMotor.Trailer.Up = OFF;
                EnableCnInt();
            } else {//������� ��������� ON  => ������� �� ��������� - �� ������
                StepMotor.Trailer.Up = ON;
                EnableCnInt();
            }
            break;
    }
    if ((StepMotor.Trailer.Down == ON) && (StepMotor.Trailer.Up == ON)) {//��� ��������� ���������� �� ���������� ���������
        StepMotor.Trailer.Alarm = ON;
        //StepMotor.MovingSettings.Mode = STOP;
        //US.OffMarker = ON;
    } //else {
       // StepMotor.Trailer.Alarm = OFF;
    //}
}
//_____________________________________________________________________________________

void StepReaction(void) {
    if (StepMotor.State.NeedStepReaction == YES) {
        if (StepMotor.MovingSettings.MovingState == PHASES_MOVING) {
            if (StepMotor.MovingSettings.TargetPosition == (signed long) StepMotor.Presets.EndPhasePoint[FOURTH]) {
                if (StepMotor.State.WorkMode == ADJUST) {
                    StepMotor.MovingSettings.MovingState = STOP; //���� ��������� �� ������� � �� �� �������
                } else {//StepMotor.WorkMode == STANDART
                    StepMotor.MovingSettings.Direction = BACK;
                    StepMotor.MovingSettings.MovingState = SIMPLE_MOVING;
                    StepMotor.MovingSettings.Speed = StepMotor.DeveloperSettings.MaxSpeed;
                    PR2 = StepMotor.MovingSettings.Speed;
                    StepMotor.MovingSettings.TargetPosition = 0;
                    StepMotor.State.MovingStepCounter = 0;
                    StepMotor_On();
                }
            } else {
                if (StepMotor.MovingSettings.TargetPosition == (signed long) StepMotor.Presets.EndPhasePoint[THIRD]) {
                    StepMotor.MovingSettings.Speed = StepMotor.Presets.PhaseSpeed[FOURTH];
                    PR2 = StepMotor.MovingSettings.Speed;
                    StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.Presets.EndPhasePoint[FOURTH];
                } else {
                    if (StepMotor.MovingSettings.TargetPosition == (signed long) StepMotor.Presets.EndPhasePoint[SECOND]) {
                        StepMotor.MovingSettings.Speed = StepMotor.Presets.PhaseSpeed[THIRD];
                        PR2 = StepMotor.MovingSettings.Speed;
                        StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.Presets.EndPhasePoint[THIRD];
                    } else {
                        if (StepMotor.MovingSettings.TargetPosition == (signed long) StepMotor.Presets.EndPhasePoint[FIRST]) {
                            StepMotor.MovingSettings.Speed = StepMotor.Presets.PhaseSpeed[SECOND];
                            PR2 = StepMotor.MovingSettings.Speed;
                            StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.Presets.EndPhasePoint[SECOND];
                        } else {
                            if (StepMotor.MovingSettings.TargetPosition == (signed long) StepMotor.Presets.EndPhasePoint[ZERO]) {
                                StepMotor.MovingSettings.Speed = StepMotor.Presets.PhaseSpeed[FIRST];
                                PR2 = StepMotor.MovingSettings.Speed;
                                StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.Presets.EndPhasePoint[FIRST];

                            }
                        }
                    }
                }
            }
        } else {
            if (StepMotor.MovingSettings.MovingState == WASHING_MOVING) {
                if (StepMotor.MovingSettings.TargetPosition == (signed long) StepMotor.Presets.EndPhasePoint[ZERO]) {
                    StepMotor.MovingSettings.TargetPosition = (signed long) StepMotor.Presets.EndPhasePoint[FOURTH];
                    StepMotor.MovingSettings.Speed = StepMotor.Presets.MaxSpeedOil;
                    PR2 = StepMotor.MovingSettings.Speed;
                }else{
                    if (StepMotor.State.WorkMode == ADJUST) {
                        StepMotor.MovingSettings.MovingState = STOP;
                    }else{
                        StepMotor.MovingSettings.Direction = BACK;
                        StepMotor.MovingSettings.MovingState = SIMPLE_MOVING;
                        StepMotor.MovingSettings.Speed = StepMotor.DeveloperSettings.MaxSpeed;
                        PR2 = StepMotor.MovingSettings.Speed;
                        StepMotor.MovingSettings.TargetPosition = 0;
                        StepMotor.State.MovingStepCounter = 0;
                        StepMotor_On();
                    }
                }
            }else{
                if(StepMotor.MovingSettings.Direction == BACK){
                    if(StepMotor.State.GeneralStepCounter == 0){
                        US_Off();
                    }
                    if(StepMotor.State.MovingStepCounter == StepMotor.Presets.ControlVolume){
                        if(StepMotor.Trailer.Up == OFF){
                            StepMotor.Trailer.Alarm = UP_TRAILER_FAIL_OFF_STATE;
                            US.OffMarker = ON;
                        }
                    }
                }
                StepMotor.MovingSettings.MovingState = STOP; //���� ��������� �� ������� � �� �� �������
            }
        }
        StepMotor.State.NeedStepReaction = NO;
    }
}
void CheckStepMotorState(void){
    if(T2CONbits.TON == 0){//�� �������� - ��������� ������������ ������ ���������
        ENABLE_LAT = 1;
        STEP_LAT = 0;
        
        StepMotor.MovingSettings.MovingState    = STOP;
        StepMotor.MovingSettings.Direction      = STOP;
        StepMotor.MovingSettings.Speed          = STOP;
        StepMotor.MovingSettings.TargetPosition = STOP;

    }else{
        if(StepMotor.State.MovingStepCounter > CHECK_STEPS){//���������� ����� ����������� ��� ���������������� ������������ ����������
            if(StepMotor.MovingSettings.Direction == FORWARD){
                if(StepMotor.Trailer.Up == ON){
                    if(StepMotor.Trailer.Alarm == OFF){
                        StepMotor.Trailer.Alarm = UP_TRAILER_FAIL_ON_STATE;
                        if(StepMotor.Trailer.UpBlock == ON){
                            StepMotor.MovingSettings.MovingState = STOP;
                            US.OffMarker = ON;
                        }
                    }
                }
            }else{
                if(StepMotor.Trailer.Down == ON){
                    if(StepMotor.Trailer.Alarm == OFF){
                        StepMotor.Trailer.Alarm = DOWN_TRAILER_FAIL_ON_STATE;
                        //US.OffMarker = ON;
                    }
//                    if(StepMotor.Trailer.DownBlock == ON){
//                        StepMotor.MovingSettings.Mode = STOP;
//                        US_Off();
//                    }
                }
            }
        }
    }

}
//_____________________________________________________________________________________
void AlarmReactionStepMotor(void) { //������� ������� �� �� �������
     unsigned char movingstate;

    if (StepMotor.MovingSettings.MovingState != STOP) {//� ��������
        if (StepMotor.MovingSettings.Direction == FORWARD) {
            movingstate = StepMotor.MovingSettings.MovingState;
            StepMotor.MovingSettings.MovingState = STOP;
            while (T2CONbits.TON == 1) {//���� ���� ����� �������� ���
            };
            StepMotor.State.NeedStepReaction = NO;
            if (StepMotor.State.WorkMode == STANDART) {
                if(movingstate == PHASES_MOVING){
                    if (StepMotor.State.GeneralStepCounter >= StepMotor.Presets.EndPhasePoint[FIRST]) {// ���� ����� ������ �� = ����������� -  ���������� �� � ������� �������
                        StepMotor.MovingSettings.Direction = BACK;
                        StepMotor.MovingSettings.MovingState = SIMPLE_MOVING;
                        StepMotor.MovingSettings.Speed = StepMotor.DeveloperSettings.MaxSpeed;
                        PR2 = StepMotor.MovingSettings.Speed;
                        StepMotor.MovingSettings.TargetPosition = 0;
                        StepMotor.State.MovingStepCounter = 0;
                        StepMotor_On();
                    }
                }
            }
        }
    }
}


