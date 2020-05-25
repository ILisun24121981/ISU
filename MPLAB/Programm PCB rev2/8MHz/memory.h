#ifndef _MEMORY_H
#define _MEMORY_H

#include "Pneumatics.h"
#include "PMTSensor.h"


//_____________________________________________________________________________________
//Определение констант

//всего 85 страниц
//1 страница = 512 инструкций(0x200)
#define PM_PAGE_SIZE_INSTR           0x200 //длинна страницы в инструкциях(512 инструкций)
//1 инструкция  = 2 адреса
#define PM_PAGE_SIZE_ADDR            PM_PAGE_SIZE_INSTR*2 //длинна страницы в адлресах  = 1024 адреса(0x400)
//1 иструкция = 4 байта, но 1 байт в инструкции - фантомный
#define PM_PAGE_SIZE_BYTE            PM_PAGE_SIZE_INSTR*4 - PM_PAGE_SIZE_INSTR //длинна страницы в значащих байтах = 1536 байта(0x600)
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
//1 строка = 64 инструкции(0x40)
#define PM_ROW_SIZE_INSTR           0x40 //длинна строки в инструкциях(64 инструкций)
//1 инструкция  = 2 адреса
#define PM_ROW_SIZE_ADDR            PM_ROW_SIZE_INSTR*2 //длинна строки в адлресах = 128 адреса(0x80)
//1 иструкция = 4 байта, но 1 байт в инструкции - фантомный
#define PM_ROW_SIZE_BYTE            PM_ROW_SIZE_INSTR*4 - PM_ROW_SIZE_INSTR //длинна страницы в значащих байтах = 192 байта(0xC0)

#define DEV_SETTINGS                1//тип данных читаемый функцией RdPmToTempBuff() - BufferType
#define DEF_SETTINGS                2//тип данных читаемый функцией RdPmToTempBuff() - BufferType
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
#define DATA_SIZE_DEV               38//Суммарное количество байт в переменных Developer Settings хранящихся в PM  (при записи настроек в память программ необходимо стереть нужное количество станиц 1536 байта - 1 страница)
#define DATA_SIZE_DEF               40//Суммарное количество байт в переменных Default Settings хранящихся в PM  (при записи настроек в память программ необходимо стереть нужное количество станиц 1536 байта - 1 страница)
//последний адрес 0x15800, но на последней странице FLASH CONFIGURATION WORDS отступаем пару страниц назад (0x15800 - 0x800) => адресс страницы с настройками 0x15000

//настройки сохраняются в область защищенную от перезаписи при прошивке через загрузчик
#define PM_DEV_SETTINGS_PAGE_ADDRESS 0x1000 //адресс начала станицы хранения настроек разработчика //
#define PM_DEF_SETTINGS_PAGE_ADDRESS 0x1400 //адресс начала станицы хранения настроек по умолчанию

// для отладки без загрузчика 
//#define PM_DEV_SETTINGS_PAGE_ADDRESS 0x15000 //82-я страница  - адресс начала станицы хранения настроек разработчика//0x15000
//#define PM_DEF_SETTINGS_PAGE_ADDRESS 0x14800 //83-я страница  - адресс начала станицы хранения настроек по умолчанию//0x14800

#define USER_PROG_RESET     0x1A00
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _//
//cmd_NVM - код операции с памятью // PIC24FJ256GA110 Family Data Sheet(5.6.1 PROGRAMMING ALGORITHM FOR FLASH PROGRAM MEMORY)
#define PM_PAGE_ERASE 		0x4042	//NVM page erase opcode
#define PM_ROW_WRITE 		0x4001	//NVM row write opcode

//_____________________________________________________________________________________
// Объявление переменных

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// определение типов
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
    Byte_t Byte[DATA_SIZE_DEV] __PACKED;//DATA_SIZE - длинна всех хранящихся в структуре переменных в байтах
    struct __PACKED {
        //при добавлении переменной увеличить константу DATA_SIZE на количество байт в переменной
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
    Byte_t Byte[DATA_SIZE_DEF] __PACKED;//DATA_SIZE - длинна всех хранящихся в структуре переменных в байтах
    struct __PACKED{
        //при добавлении переменной увеличить константу DATA_SIZE на количество байт в переменной
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
// Объявление переменных
extern DevelorerSettingBuffer_t DevSetBuffer;//временный буфер для хранения настроек разработчика (см. комментарий внизу файла)
extern DefaultSettingBuffer_t DefSetBuffer;//временный буфер для хранения настроек разработчика (см. комментарий внизу файла)
extern DoubleWord_Value_t SourseAddr;
extern unsigned short int DataSize;
extern unsigned char BufferType;

//_____________________________________________________________________________________
// Объявление функций

void RdPmToTempBuff(DoubleWord_Value_t sourseaddress,unsigned int datasize,unsigned char buffertype);
DoubleWord_t ReadLatch(Word_t page, Word_t addrLo);
void WrTempBuffToPm(DoubleWord_Value_t sourseaddress,unsigned int datasize,unsigned char buffertype);
void WriteLatch(Word_t page, Word_t addrLo, Word_t dataHi, Word_t dataLo);
void WriteRowPM(void);
void ErasePage(Word_t page, Word_t addrLo);
void InitPMSettings(void);

#endif//END #ifndef _MEMORY_H

