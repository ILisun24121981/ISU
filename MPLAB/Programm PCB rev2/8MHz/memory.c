#include "memory.h"
#include "main.h"
#include "StepMotorProba.h"
#include "SM1150.h"
#include "pwm.h"
#include "Pneumatics.h"
#include "uart.h"
#include "packet.h"
#include "StepMotorPMT.h"

 DevelorerSettingBuffer_t DevSetBuffer;//временный буфер для хранения настроек разработчика (см. комментарий внизу файла)
 DefaultSettingBuffer_t DefSetBuffer;
 DoubleWord_Value_t SourseAddr;
 unsigned short int DataSize;
 unsigned char BufferType;

#ifdef USE_RUNAWAY_PROTECT
    volatile Word_t writeKey1 = 0xFFFF;
    volatile Word_t writeKey2 = 0x5555;
    volatile Word_t keyTest1 = 0x0000;
    volatile Word_t keyTest2 = 0xAAAA;
#endif
//Функции написаны в соответствии с PIC24FJ256GA110 Family Data Sheet (5.6.1 PROGRAMMING ALGORITHM FOR FLASH PROGRAM MEMORY), используя пример написания Bootloader AN1094 Microchip
//_____________________________________________________________________________________

void RdPmToTempBuff(DoubleWord_Value_t sourseaddress,unsigned int datasize,unsigned char buffertype) {//функция считывания настроек разработчика из памяти программ(PM)во временный буфер DevSetBuffer
    unsigned char i = 0;
    unsigned short int bufferbyte = 0; //
    DoubleWord_Value_t temp;
  
    temp.Value = 0;
    while (bufferbyte < datasize) {
        temp.Value = ReadLatch(sourseaddress.Words.HW, sourseaddress.Words.LW); //считать инструкцию
        while (i < 3) {
            if (bufferbyte < datasize) {
                if(buffertype == DEV_SETTINGS){
                    DevSetBuffer.Byte[bufferbyte] = temp.Byte[i];//для работоспособности данного кода необходимо размещать переменные типа CHAR в конце структуры DevSetBuffer.Struct
                }
                if(buffertype == DEF_SETTINGS){
                    DefSetBuffer.Byte[bufferbyte] = temp.Byte[i];//для работоспособности данного кода необходимо размещать переменные типа CHAR в конце структуры DefSetBuffer.Struct
                }
                i++;
                bufferbyte++;
            }else{
                i=3;
            }
        }
        i = 0;
        sourseaddress.Value += 2; //адресс следующей инструкции
    }
}//END ReadDevSettingsToTempBuffer()

//_____________________________________________________________________________________

DoubleWord_t ReadLatch(Word_t page, Word_t addrLo) {//функция считывания инструкции PM
    DoubleWord_Value_t temp;

    temp.Value = 0;
    TBLPAG = page;
    temp.Words.LW = __builtin_tblrdl(addrLo);
    temp.Words.HW = __builtin_tblrdh(addrLo);
    return temp.Value;
}//END ReadLatch

//_____________________________________________________________________________________

void WrTempBuffToPm(DoubleWord_Value_t sourseaddress,unsigned int datasize,unsigned char buffertype) {//функция записи из временного буфера
    unsigned char i = 0;
    unsigned char n = 1;//номер записываемой строки
    unsigned short int bufferbyte = 0;
    DoubleWord_Value_t startaddresss;
    DoubleWord_Value_t temp;

    startaddresss.Value = sourseaddress.Value;
    temp.Value = 0;
    while(bufferbyte < datasize) {
        while (sourseaddress.Value < (startaddresss.Value + (PM_ROW_SIZE_ADDR*n))) {//проверка записи защелки строки
            if (bufferbyte < datasize) {
                if(buffertype == DEV_SETTINGS){
                    temp.Byte[i] = DevSetBuffer.Byte[bufferbyte];
                }
                if(buffertype == DEF_SETTINGS){
                    temp.Byte[i] = DefSetBuffer.Byte[bufferbyte];
                }
                bufferbyte++;
            } else {
                temp.Byte[i] = 0x0000;
            }
            i++;            
            if (i == 3) {
                WriteLatch(sourseaddress.Words.HW, sourseaddress.Words.LW, temp.Words.HW, temp.Words.LW);//запись защелки инструкции
                sourseaddress.Value+=2;
                temp.Value = 0;
                i=0;
            }
        }
        WriteRowPM();//запись защелки строки
        n++;
    }
}//END WrTempDevSetBuffToPmDevSet()

//_____________________________________________________________________________________

void WriteLatch(Word_t page, Word_t addrLo, Word_t dataHi, Word_t dataLo) {//функция записи защелки инструкции
    TBLPAG = page;
    __builtin_tblwtl(addrLo, dataLo);
    __builtin_tblwth(addrLo, dataHi);
}//END WriteLatch()

//_____________________________________________________________________________________

void WriteRowPM(void) {//функция записи защелки строки в память программ PM
    NVMCON = PM_ROW_WRITE;
    __builtin_write_NVM();
    while (NVMCONbits.WR == 1);
}//END WriteRowPM()

//_____________________________________________________________________________________

void ErasePage(Word_t page, Word_t addrLo) {//функция стирания станицы PM
    Word_t temp = 0;

    temp = TBLPAG;
    TBLPAG = page;
    NVMCON = PM_PAGE_ERASE;
    __builtin_tblwtl(addrLo, 0x0000);
    asm("DISI #5"); // Block all interrupts with priority <7 for next 5 instructions
    __builtin_write_NVM(); //C30 function to perform unlock sequence and set WR
    while (NVMCONbits.WR == 1) {
    };
    TBLPAG = temp;
}//END ErasePage()

//_____________________________________________________________________________________

void InitPMSettings(void){// функция задания настроек разработчика
    unsigned char i;
    //Загрузка настроек разработчика
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    
    SourseAddr.Value = PM_DEV_SETTINGS_PAGE_ADDRESS;
    DataSize = DATA_SIZE_DEV;
    BufferType = DEV_SETTINGS;
    RdPmToTempBuff(SourseAddr,DataSize,BufferType);
    //Считали настройки разработчика

    // Настройки ШД
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    StepMotor.DeveloperSettings.MaxSpeed = DevSetBuffer.Value.SM_MaxSpeed;
    StepMotor.DeveloperSettings.TransferCoefficientSL = DevSetBuffer.Value.SM_TransferCoefficientSL;
    StepMotor.DeveloperSettings.MaxControlVolume = DevSetBuffer.Value.SM_MaxControlVolume;
    
    //Присвоили настройки разработчика
    if(StepMotor.DeveloperSettings.MaxSpeed == 0xFFFF){
       StepMotor.DeveloperSettings.MaxSpeed = DEFAULT_MIN_SPEED;// значение PR2 при котором скорость равна 2шаг/сек
    }
    if(StepMotor.DeveloperSettings.TransferCoefficientSL == 0xFFFF){
       StepMotor.DeveloperSettings.TransferCoefficientSL = DEFAULT_TRANSFERT_COEFF_SL;//
    }
    if(StepMotor.DeveloperSettings.MaxControlVolume == 0xFFFF){
       StepMotor.DeveloperSettings.MaxControlVolume = DEFAULT_MAX_MAX_CONTROL_VOLUME;//
    }
    //подкорректировали для вывода в случае если это первоначальная (разработческая) настройка (обратить внимаение на ограничения!)

    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    StepMotor1.State.SavedPosition = DevSetBuffer.Value.SM1_SavedPosition;
    StepMotor1.State.InSavedPositionMarker = DevSetBuffer.Value.SM1_InSavedPositionMarker;
    if(StepMotor1.State.InSavedPositionMarker == YES){
            StepMotor1.State.ZeroSetMarker = YES;
            StepMotor1.State.GeneralStepCounter = StepMotor1.State.SavedPosition;
    }else{
       StepMotor1.State.InSavedPositionMarker = NO;//на случай если там 0xFFFF
       StepMotor1.State.ZeroSetMarker = NO;
       StepMotor1.State.GeneralStepCounter = 0;
    }
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Настройки SM1150
    i=0;
    while(i<24){
        SM1150.Dev_SM1150_Coeff[i] = DevSetBuffer.Value.SM1150_Coeff[i];
        i++;
    }


    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
//    SM1150.DeveloperSettings.PWM_Period = DevSetBuffer.Value.SM1150_PWM_Period;
//    if(SM1150.DeveloperSettings.PWM_Period == 0xFFFF){
//        SM1150.DeveloperSettings.PWM_Period = DEFAULT_PWM_PERIOD;//=50000
//    }
//    i = POWER_LEVEL_0;
//    while(i<= POWER_LEVEL_30){
//       SM1150.DeveloperSettings.Power_Level[0][i] =  DevSetBuffer.Value.SM1150_Power_Level[0][i];
//       SM1150.DeveloperSettings.Power_Level[1][i] =  DevSetBuffer.Value.SM1150_Power_Level[1][i];
//       if(SM1150.DeveloperSettings.Power_Level[1][i] == 0xFFFF){
//           SM1150.DeveloperSettings.Power_Level[0][i] = DEFAULT_SM1150_POWER;//=0
//           SM1150.DeveloperSettings.Power_Level[1][i] = DEFAULT_PWM_DUTY_CYCLE;//=0
//       }
//       i++;
//    }
//    SM1150.DeveloperSettings.Min_Power_Level_Number = DevSetBuffer.Value.Min_Power_Level_Number;
//    if(SM1150.DeveloperSettings.Min_Power_Level_Number == 0xFF ){
//        SM1150.DeveloperSettings.Min_Power_Level_Number = DEFAULT_MIN_POWER_LEVEL;
//    }
//    SM1150.DeveloperSettings.Max_Power_Level_Number = DevSetBuffer.Value.Max_Power_Level_Number;
//    if(SM1150.DeveloperSettings.Max_Power_Level_Number == 0xFF){
//        SM1150.DeveloperSettings.Max_Power_Level_Number = DEFAULT_MAX_POWER_LEVEL;
//    }

    //Загрузка настроек по умолчанию. Установка данных настроек в качестве текущих
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    SourseAddr.Value = PM_DEF_SETTINGS_PAGE_ADDRESS;
    DataSize = DATA_SIZE_DEF;
    BufferType = DEF_SETTINGS;
    RdPmToTempBuff(SourseAddr,DataSize,BufferType);
    //Считали настройки по умолчанию
    
    // Настройки ШД
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //Загрузим настройки по умолчанию
    StepMotor.DefaultPresets.MaxSpeedOil = DefSetBuffer.Value.SM_MaxSpeedOil;
    StepMotor.DefaultPresets.TransferCoefficientLV = DefSetBuffer.Value.SM_TransferCoefficientLV;
    StepMotor.DefaultPresets.ControlVolume = DefSetBuffer.Value.SM_ControlVolume;
    
    
    if(StepMotor.DefaultPresets.MaxSpeedOil == 0xFFFF){
       StepMotor.DefaultPresets.MaxSpeedOil = StepMotor.DeveloperSettings.MaxSpeed;// значение PR2 при котором скорость равна 2шаг/сек
    }
     if(StepMotor.DefaultPresets.TransferCoefficientLV == 0xFFFF){
        StepMotor.DefaultPresets.TransferCoefficientLV = DEFAULT_TRANSFERT_COEFF_LV;
    }
    if(StepMotor.DefaultPresets.ControlVolume == 0xFFFF){
       StepMotor.DefaultPresets.ControlVolume = StepMotor.DeveloperSettings.MaxControlVolume;
    }
    
    i=0;
    while(i<=FOURTH){//присвоим настройки фаз с учетом ограничений на ввод для корректного отображения
        StepMotor.DefaultPresets.EndPhasePoint[i] = DefSetBuffer.Value.SM_EndPhasePoint[i];
        if(StepMotor.DefaultPresets.EndPhasePoint[i] > DEFAULT_MAX_MAX_CONTROL_VOLUME){
           StepMotor.DefaultPresets.EndPhasePoint[i] = DEFAULT_MAX_MAX_CONTROL_VOLUME;
        }
        StepMotor.DefaultPresets.PhaseSpeed[i] = DefSetBuffer.Value.SM_PhaseSpeed[i];
        if(StepMotor.DefaultPresets.PhaseSpeed[i] > DEFAULT_MIN_SPEED){
           StepMotor.DefaultPresets.PhaseSpeed[i] = DEFAULT_MIN_SPEED;
        }
        i++;
    }

    //загрузим дополнительные текущие настройки
    StepMotor.Presets.MaxSpeedOil = StepMotor.DefaultPresets.MaxSpeedOil;
    StepMotor.Presets.TransferCoefficientLV = StepMotor.DefaultPresets.TransferCoefficientLV;
    StepMotor.Presets.ControlVolume = StepMotor.DefaultPresets.ControlVolume;
    //загрузим текущие настройки фаз
    i = 0;
    while (i <= FOURTH) {
        StepMotor.Presets.EndPhasePoint[i] = StepMotor.DefaultPresets.EndPhasePoint[i];
        StepMotor.Presets.PhaseSpeed[i] = StepMotor.DefaultPresets.PhaseSpeed[i];
        i++;
    }

    //Исправим загруженные значения дополнительные текущих настроек на случай измененных инженерных настроек
    if (StepMotor.Presets.ControlVolume > StepMotor.DeveloperSettings.MaxControlVolume) {
        StepMotor.Presets.ControlVolume = StepMotor.DeveloperSettings.MaxControlVolume;
    }
    if (StepMotor.Presets.MaxSpeedOil < StepMotor.DeveloperSettings.MaxSpeed) {
        StepMotor.Presets.MaxSpeedOil = StepMotor.DeveloperSettings.MaxSpeed;
    }

    //исправим загруженные значения текущиx настроек фаз в соответствии с исправленными значениями дополнительных текущих настроек
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








//    //Присвоим текущим дополнительным настройкам  настройки по умолчанию
//    StepMotor.Presets.MaxSpeedOil = StepMotor.DefaultPresets.MaxSpeedOil;
//    StepMotor.Presets.TransferCoefficientLV = StepMotor.DefaultPresets.TransferCoefficientLV;
//    StepMotor.Presets.ControlVolume = StepMotor.DefaultPresets.ControlVolume;
//    //Исправим в соответствии с настройками разработчика
//    if(StepMotor.Presets.ControlVolume > StepMotor.DeveloperSettings.MaxControlVolume ){
//        StepMotor.Presets.ControlVolume = StepMotor.DeveloperSettings.MaxControlVolume;
//    }
//    if(StepMotor.Presets.MaxSpeedOil < StepMotor.DeveloperSettings.MaxSpeed ){
//        StepMotor.Presets.MaxSpeedOil = StepMotor.DeveloperSettings.MaxSpeed ;
//    }
//    //откорректируем
//    i=ZERO;
//    while(i<=FOURTH){
//        if(StepMotor.DefaultPresets.EndPhasePoint[i] < StepMotor.Presets.ControlVolume){
//            StepMotor.Presets.EndPhasePoint[i] = StepMotor.DefaultPresets.EndPhasePoint[i];
//            if(StepMotor.DefaultPresets.PhaseSpeed[i] < StepMotor.Presets.MaxSpeedOil ){
//                StepMotor.Presets.PhaseSpeed[i] = StepMotor.Presets.MaxSpeedOil;
//            }else{
//                StepMotor.Presets.PhaseSpeed[i] = StepMotor.DefaultPresets.PhaseSpeed[i];
//            }
//        }else{
//            StepMotor.Presets.EndPhasePoint[i] = StepMotor.Presets.ControlVolume;
//            if(StepMotor.DefaultPresets.PhaseSpeed[i] < StepMotor.Presets.MaxSpeedOil ){
//                StepMotor.Presets.PhaseSpeed[i] = StepMotor.Presets.MaxSpeedOil;
//            }else{
//                StepMotor.Presets.PhaseSpeed[i] = StepMotor.DefaultPresets.PhaseSpeed[i];
//            }
//            i++;
//            while(i<=FOURTH){
//                 StepMotor.Presets.EndPhasePoint[i] = StepMotor.Presets.ControlVolume;
//                 StepMotor.Presets.PhaseSpeed[i] = DEFAULT_MIN_SPEED;
//                 i++;
//            }
//        }
//        i++;
    //Присвоили текущие настройки фаз в соответствии с настройками фаз по умолчанию и исправленными дополнительными текущими настройками
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _

    // Настройки SM1150
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    SM1150.Default_Power_Level = DefSetBuffer.Value.SM1150_Power_Level;
    if(SM1150.Default_Power_Level == 0xFFFF){
        SM1150.Default_Power_Level = DEFAULT_PWM_DUTY_CYCLE;
    }

    SM1150.Power_Level = SM1150.Default_Power_Level;
    
//    if(SM1150.Power_Level > SM1150.DeveloperSettings.Power_Level[1][SM1150.DeveloperSettings.Max_Power_Level_Number]){ //защита от неправильного считывания из PM и корректировка загружаемого значения в соответствии с новыми установками разработчика
//        SM1150.Power_Level = SM1150.DeveloperSettings.Power_Level[1][SM1150.DeveloperSettings.Max_Power_Level_Number];
//    }
//    if(SM1150.Power_Level < SM1150.DeveloperSettings.Power_Level[1][SM1150.DeveloperSettings.Min_Power_Level_Number]){ //защита от неправильного считывания из PM и корректировка загружаемого значения в соответствии с новыми установками разработчика
//        SM1150.Power_Level = SM1150.DeveloperSettings.Power_Level[1][SM1150.DeveloperSettings.Min_Power_Level_Number];
//    }

    // Настройки пневматической системы клапанов
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    Pneumatics.DefaultValveInTime = DefSetBuffer.Value.Pneumatics_ValveInTime;
    if(Pneumatics.DefaultValveInTime == 0xFFFF){
        Pneumatics.DefaultValveInTime = DEFAULT_VALVE_IN_TIME;
    }
    Pneumatics.ValveInTime = Pneumatics.DefaultValveInTime;

    Pneumatics.DefaultValveOutTime = DefSetBuffer.Value.Pneumatics_ValveOutTime;
    if(Pneumatics.DefaultValveOutTime == 0xFFFF){
        Pneumatics.DefaultValveOutTime = DEFAULT_VALVE_OUT_TIME;
    }
    Pneumatics.ValveOutTime = Pneumatics.DefaultValveOutTime;

    Pneumatics.DefaultNumberOfCycles = DefSetBuffer.Value.Pneumatics_NumberOfCycles;
    if(Pneumatics.DefaultNumberOfCycles == 0xFFFF){
        Pneumatics.DefaultNumberOfCycles = DEFAULT_NIMBER_OF_CYCLES;
    }
    Pneumatics.NumberOfCycles = Pneumatics.DefaultNumberOfCycles;

    // Настройки датчика ФЭУ (горения плазмы)
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    PMTSensor.DefaultAverageNumberForPlasmaDetect = DefSetBuffer.Value.PMTSensor_AverageNumberForPlasmaDetect;
    if(PMTSensor.DefaultAverageNumberForPlasmaDetect == 0xFFFF){
        PMTSensor.DefaultAverageNumberForPlasmaDetect = DEFAULT_AVERAGE_NUMBER_FO_PLASMA_DETECT;
    }
    PMTSensor.AverageNumberForPlasmaDetect = PMTSensor.DefaultAverageNumberForPlasmaDetect;

    PMTSensor.DefaultDecisionLevel = DefSetBuffer.Value.PMTSensor_DecisionLevel;
    if(PMTSensor.DefaultDecisionLevel == 0xFFFF){
        PMTSensor.DefaultDecisionLevel = DEFAULT_DECISION_LEVEL;
    }    
    PMTSensor.DecisionLevel = PMTSensor.DefaultDecisionLevel;

    PMTSensor.DefaultHeatingTime = DefSetBuffer.Value.PMTSensor_HeartingTime;
    if(PMTSensor.DefaultHeatingTime == 0xFFFF){
        PMTSensor.DefaultHeatingTime = DEFAULT_HEATING_TIME;
    }
    PMTSensor.HeatingTime = PMTSensor.DefaultHeatingTime;
    PMTSensor.HeatingTimerSec = PMTSensor.HeatingTime;
}
void CommandMemory(void){
    unsigned char i;
    if (Uart.Rx.Packet.Command == EXPORT_PACKET) {

        StepMotor.MovingSettings.MovingState = STOP;
        while (T2CONbits.TON == 1) {//ждем пока будет закончен шаг
        };
        StepMotor.State.NeedStepReaction = NO;

        SourseAddr.Value = PM_DEV_SETTINGS_PAGE_ADDRESS;
        DataSize = DATA_SIZE_DEV;
        BufferType = DEV_SETTINGS;
        SRbits.IPL = 7;
        RdPmToTempBuff(SourseAddr,DataSize,BufferType);
        SourseAddr.Value = PM_DEF_SETTINGS_PAGE_ADDRESS;
        DataSize = DATA_SIZE_DEF;
        BufferType = DEF_SETTINGS;
        RdPmToTempBuff(SourseAddr,DataSize,BufferType);
        SRbits.IPL = 0;

        StartTxPacket(0, EXPORT_PACKET);
        MoveDataToTxPacket (CHAR,DevSetBuffer.Value.Equipment_Number);
        MoveDataToTxPacket (CHAR,DevSetBuffer.Value.PCB_Version);
        MoveDataToTxPacket (CHAR,DevSetBuffer.Value.Assembly_Version);
        MoveDataToTxPacket (CHAR,DevSetBuffer.Value.Firmware_Version);
        MoveDataToTxPacket (CHAR,DevSetBuffer.Value.Settings_File_Version);

        MoveDataToTxPacket (INT,DevSetBuffer.Value.SM_MaxSpeed);
        MoveDataToTxPacket (INT,DevSetBuffer.Value.SM_TransferCoefficientSL);
        MoveDataToTxPacket (INT,DevSetBuffer.Value.SM_MaxControlVolume);
        //MoveDataToTxPacket (INT,DevSetBuffer.Value.SM1150_PWM_Period);

        MoveDataToTxPacket (INT,DefSetBuffer.Value.SM_MaxSpeedOil);
        MoveDataToTxPacket (INT,DefSetBuffer.Value.SM_TransferCoefficientLV);
        MoveDataToTxPacket (INT,DefSetBuffer.Value.SM_ControlVolume);
        MoveDataToTxPacket (INT,DefSetBuffer.Value.SM_EndPhasePoint[0]);
        MoveDataToTxPacket (INT,DefSetBuffer.Value.SM_EndPhasePoint[1]);
        MoveDataToTxPacket (INT,DefSetBuffer.Value.SM_EndPhasePoint[2]);
        MoveDataToTxPacket (INT,DefSetBuffer.Value.SM_EndPhasePoint[3]);
        MoveDataToTxPacket (INT,DefSetBuffer.Value.SM_EndPhasePoint[4]);
        MoveDataToTxPacket (INT,DefSetBuffer.Value.SM_PhaseSpeed[0]);
        MoveDataToTxPacket (INT,DefSetBuffer.Value.SM_PhaseSpeed[1]);
        MoveDataToTxPacket (INT,DefSetBuffer.Value.SM_PhaseSpeed[2]);
        MoveDataToTxPacket (INT,DefSetBuffer.Value.SM_PhaseSpeed[3]);
        MoveDataToTxPacket (INT,DefSetBuffer.Value.SM_PhaseSpeed[4]);

        MoveDataToTxPacket (INT,DefSetBuffer.Value.SM1150_Power_Level);

        MoveDataToTxPacket (INT,DefSetBuffer.Value.Pneumatics_ValveInTime);
        MoveDataToTxPacket (INT,DefSetBuffer.Value.Pneumatics_ValveOutTime);
        MoveDataToTxPacket (INT,DefSetBuffer.Value.Pneumatics_NumberOfCycles);

        MoveDataToTxPacket (INT,DefSetBuffer.Value.PMTSensor_AverageNumberForPlasmaDetect);
        MoveDataToTxPacket (INT,DefSetBuffer.Value.PMTSensor_DecisionLevel);
        MoveDataToTxPacket (INT,DefSetBuffer.Value.PMTSensor_HeartingTime);

        i=0;
        while(i<24){
           MoveDataToTxPacket(CHAR,SM1150.Dev_SM1150_Coeff[i]);
           i++;
        }

        EndTxPacket();
        SendTxPaket();
    }
    if (Uart.Rx.Packet.Command == IMPORT_PACKET) {
        
        StepMotor.MovingSettings.MovingState = STOP;
        while (T2CONbits.TON == 1) {//ждем пока будет закончен шаг
        };
        StepMotor.State.NeedStepReaction = NO;
        
        SourseAddr.Value = PM_DEV_SETTINGS_PAGE_ADDRESS;
        DataSize = DATA_SIZE_DEV;
        BufferType = DEV_SETTINGS;
        SRbits.IPL = 7;
        RdPmToTempBuff(SourseAddr,DataSize,BufferType);
        SourseAddr.Value = PM_DEF_SETTINGS_PAGE_ADDRESS;
        DataSize = DATA_SIZE_DEF;
        BufferType = DEF_SETTINGS;
        RdPmToTempBuff(SourseAddr,DataSize,BufferType);
        SRbits.IPL = 0;
        
        StartReadRxData();

        ReadDataFromRxPacket(CHAR);
        DevSetBuffer.Value.Equipment_Number = Uart.Rx.RxPackDataToRead.Char[0];
        ReadDataFromRxPacket(CHAR);
        DevSetBuffer.Value.PCB_Version = Uart.Rx.RxPackDataToRead.Char[0];
        ReadDataFromRxPacket(CHAR);
        DevSetBuffer.Value.Assembly_Version = Uart.Rx.RxPackDataToRead.Char[0];
        ReadDataFromRxPacket(CHAR);
        DevSetBuffer.Value.Firmware_Version = Uart.Rx.RxPackDataToRead.Char[0];
        ReadDataFromRxPacket(CHAR);
        DevSetBuffer.Value.Settings_File_Version = Uart.Rx.RxPackDataToRead.Char[0];
        
        ReadDataFromRxPacket(INT);
        DevSetBuffer.Value.SM_MaxSpeed = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DevSetBuffer.Value.SM_TransferCoefficientSL = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DevSetBuffer.Value.SM_MaxControlVolume = Uart.Rx.RxPackDataToRead.Int[0];
        //ReadDataFromRxPacket(INT);
        //DevSetBuffer.Value.SM1150_PWM_Period = Uart.Rx.RxPackDataToRead.Int[0];
        
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.SM_MaxSpeedOil = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.SM_TransferCoefficientLV = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.SM_ControlVolume = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.SM_EndPhasePoint[0] = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.SM_EndPhasePoint[1] = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.SM_EndPhasePoint[2] = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.SM_EndPhasePoint[3] = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.SM_EndPhasePoint[4] = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.SM_PhaseSpeed[0] = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.SM_PhaseSpeed[1] = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.SM_PhaseSpeed[2] = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.SM_PhaseSpeed[3] = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.SM_PhaseSpeed[4] = Uart.Rx.RxPackDataToRead.Int[0];
        
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.SM1150_Power_Level = Uart.Rx.RxPackDataToRead.Int[0];
        
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.Pneumatics_ValveInTime = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.Pneumatics_ValveOutTime = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.Pneumatics_NumberOfCycles = Uart.Rx.RxPackDataToRead.Int[0];
        
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.PMTSensor_AverageNumberForPlasmaDetect = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.PMTSensor_DecisionLevel = Uart.Rx.RxPackDataToRead.Int[0];
        ReadDataFromRxPacket(INT);
        DefSetBuffer.Value.PMTSensor_HeartingTime = Uart.Rx.RxPackDataToRead.Int[0];
        
        i=0;
        while(i<24){
           ReadDataFromRxPacket(CHAR);
           DevSetBuffer.Value.SM1150_Coeff[i] = Uart.Rx.RxPackDataToRead.Char[0]; 
           i++;         
        }
       
        SourseAddr.Value = PM_DEV_SETTINGS_PAGE_ADDRESS;
        DataSize = DATA_SIZE_DEV;
        BufferType = DEV_SETTINGS;
        SRbits.IPL = 7;
        ErasePage(SourseAddr.Words.HW, SourseAddr.Words.LW);
        WrTempBuffToPm(SourseAddr, DataSize, BufferType);
        
        SourseAddr.Value = PM_DEF_SETTINGS_PAGE_ADDRESS;
        DataSize = DATA_SIZE_DEF;
        BufferType = DEF_SETTINGS;
        ErasePage(SourseAddr.Words.HW, SourseAddr.Words.LW);
        WrTempBuffToPm(SourseAddr, DataSize, BufferType);
        SRbits.IPL = 0;

        InitPMSettings();
    }    
}
