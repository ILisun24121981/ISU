#ifndef _MEMORY_H
#define _MEMORY_H

#include "Pneumatics.h"
#include "PMTSensor.h"


//_____________________________________________________________________________________
//����������� ��������

//����� 85 �������
//1 �������� = 512 ����������(0x200)
#define PM_PAGE_SIZE_INSTR           0x200 //������ �������� � �����������(512 ����������)
//1 ����������  = 2 ������
#define PM_PAGE_SIZE_ADDR            PM_PAGE_SIZE_INSTR*2 //������ �������� � ��������  = 1024 ������(0x400)
//1 ��������� = 4 �����, �� 1 ���� � ���������� - ���������
#define PM_PAGE_SIZE_BYTE            PM_PAGE_SIZE_INSTR*4 - PM_PAGE_SIZE_INSTR //������ �������� � �������� ������ = 1536 �����(0x600)
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
//1 ������ = 64 ����������(0x40)
#define PM_ROW_SIZE_INSTR           0x40 //������ ������ � �����������(64 ����������)
//1 ����������  = 2 ������
#define PM_ROW_SIZE_ADDR            PM_ROW_SIZE_INSTR*2 //������ ������ � �������� = 128 ������(0x80)
//1 ��������� = 4 �����, �� 1 ���� � ���������� - ���������
#define PM_ROW_SIZE_BYTE            PM_ROW_SIZE_INSTR*4 - PM_ROW_SIZE_INSTR //������ �������� � �������� ������ = 192 �����(0xC0)

#define DEV_SETTINGS                1//��� ������ �������� �������� RdPmToTempBuff() - BufferType
#define DEF_SETTINGS                2//��� ������ �������� �������� RdPmToTempBuff() - BufferType
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
#define DATA_SIZE_DEV               38//��������� ���������� ���� � ���������� Developer Settings ���������� � PM  (��� ������ �������� � ������ �������� ���������� ������� ������ ���������� ������ 1536 ����� - 1 ��������)
#define DATA_SIZE_DEF               40//��������� ���������� ���� � ���������� Default Settings ���������� � PM  (��� ������ �������� � ������ �������� ���������� ������� ������ ���������� ������ 1536 ����� - 1 ��������)
//��������� ����� 0x15800, �� �� ��������� �������� FLASH CONFIGURATION WORDS ��������� ���� ������� ����� (0x15800 - 0x800) => ������ �������� � ����������� 0x15000

//��������� ����������� � ������� ���������� �� ���������� ��� �������� ����� ���������
#define PM_DEV_SETTINGS_PAGE_ADDRESS 0x1000 //������ ������ ������� �������� �������� ������������ //
#define PM_DEF_SETTINGS_PAGE_ADDRESS 0x1400 //������ ������ ������� �������� �������� �� ���������

// ��� ������� ��� ���������� 
//#define PM_DEV_SETTINGS_PAGE_ADDRESS 0x15000 //82-� ��������  - ������ ������ ������� �������� �������� ������������//0x15000
//#define PM_DEF_SETTINGS_PAGE_ADDRESS 0x14800 //83-� ��������  - ������ ������ ������� �������� �������� �� ���������//0x14800

#define USER_PROG_RESET     0x1A00
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _//
//cmd_NVM - ��� �������� � ������� // PIC24FJ256GA110 Family Data Sheet(5.6.1 PROGRAMMING ALGORITHM FOR FLASH PROGRAM MEMORY)
#define PM_PAGE_ERASE 		0x4042	//NVM page erase opcode
#define PM_ROW_WRITE 		0x4001	//NVM row write opcode

//_____________________________________________________________________________________
// ���������� ����������

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ����������� �����
typedef unsigned char           Byte_t;       //16-bit unsigned
typedef unsigned short int      Word_t;       //16-bit unsigned
typedef unsigned long           DoubleWord_t; //32-bit unsigned

typedef union
{
    DoubleWord_t Value;
    Word_t Word[2] __PACKED;
    Byte_t Byte[4]__PACKED;
    struct __PACKED{
        Word_t LW;
        Word_t HW;
    } Words;
} DoubleWord_Value_t;

typedef union{
    Byte_t Byte[DATA_SIZE_DEV] __PACKED;//DATA_SIZE - ������ ���� ���������� � ��������� ���������� � ������
    struct __PACKED {
        //��� ���������� ���������� ��������� ��������� DATA_SIZE �� ���������� ���� � ����������
        unsigned short int SM_MaxSpeed;
        unsigned short int SM_TransferCoefficientSL;
        unsigned short int SM_MaxControlVolume;

        signed short int SM1_SavedPosition;
        unsigned char SM1_InSavedPositionMarker;

        unsigned char SM1150_Coeff[24];

        unsigned char Equipment_Number;
        unsigned char PCB_Version;
        unsigned char Assembly_Version;
        unsigned char Firmware_Version;
        unsigned char Settings_File_Version;
    }Value;
} DevelorerSettingBuffer_t;

typedef union{
    Byte_t Byte[DATA_SIZE_DEF] __PACKED;//DATA_SIZE - ������ ���� ���������� � ��������� ���������� � ������
    struct __PACKED{
        //��� ���������� ���������� ��������� ��������� DATA_SIZE �� ���������� ���� � ����������
        unsigned short int SM_MaxSpeedOil;
        unsigned short int SM_TransferCoefficientLV;
        unsigned short int SM_ControlVolume;
        unsigned short int SM_EndPhasePoint[5];
        unsigned short int SM_PhaseSpeed[5];

        unsigned short int SM1150_Power_Level;

        unsigned short int Pneumatics_ValveInTime;
        unsigned short int Pneumatics_ValveOutTime;
        unsigned short int Pneumatics_NumberOfCycles;

        unsigned short int PMTSensor_AverageNumberForPlasmaDetect;
        unsigned short int PMTSensor_DecisionLevel;
        unsigned short int PMTSensor_HeartingTime;
    }Value;
} DefaultSettingBuffer_t;
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ���������� ����������
extern DevelorerSettingBuffer_t DevSetBuffer;//��������� ����� ��� �������� �������� ������������ (��. ����������� ����� �����)
extern DefaultSettingBuffer_t DefSetBuffer;//��������� ����� ��� �������� �������� ������������ (��. ����������� ����� �����)
extern DoubleWord_Value_t SourseAddr;
extern unsigned short int DataSize;
extern unsigned char BufferType;

//_____________________________________________________________________________________
// ���������� �������

void RdPmToTempBuff(DoubleWord_Value_t sourseaddress,unsigned int datasize,unsigned char buffertype);
DoubleWord_t ReadLatch(Word_t page, Word_t addrLo);
void WrTempBuffToPm(DoubleWord_Value_t sourseaddress,unsigned int datasize,unsigned char buffertype);
void WriteLatch(Word_t page, Word_t addrLo, Word_t dataHi, Word_t dataLo);
void WriteRowPM(void);
void ErasePage(Word_t page, Word_t addrLo);
void InitPMSettings(void);

#endif//END #ifndef _MEMORY_H

