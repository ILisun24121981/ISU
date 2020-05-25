#include "main.h"


#include "crc.h"
#include "packet.h"

#include "uart.h"
#include "timer.h"
#include "adc.h"
#include "pwm.h"

#include "interruptpriority.h"
#include "cn.h"

#include "memory.h"

#include "StepMotorProba.h"
#include "SM1150.h"
#include "Pneumatics.h"
#include "US.h"

#include "GasPressureSensor.h"
#include "GasExtractSensor.h"
#include "GasPlasmaFlowSensor.h"
#include "GasTransportFlowSensor.h"
#include "PMTSensor.h"
#include "StepMotorPMT.h"

//_CONFIG2(IESO_ON  & FNOSC_FRCPLL & FCKSM_CSDCMD & OSCIOFNC_ON & IOL1WAY_OFF & POSCMOD_NONE);
//_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & ICS_PGx1 & FWDTEN_OFF & WINDIS_OFF & FWPSA_PR32 & WDTPS_PS32768);

int CONFIG3 __attribute__((space(prog), address(0x157FA))) = 0xFFFF;
//_CONFIG3(
//    WPFP_WPFP511 &       // Write Protection Flash Page Segment Boundary (Highest Page (same as page 85))
//    WPDIS_WPDIS &        // Segment Write Protection Disable bit (Segmented code protection disabled)
//    WPCFG_WPCFGDIS &     // Configuration Word Code Page Protection Select bit (Last page(at the top of program memory) and Flash configuration words are not protected)
//    WPEND_WPENDMEM       // Segment Write Protection End Page Select bit (Write Protect from WPFP to the last page of memory)
//);
int CONFIG2 __attribute__((space(prog), address(0x157FC))) = 0xF18F;
//_CONFIG2(
//    POSCMOD_NONE &       // Primary Oscillator Select (Primary oscillator disabled)
//    IOL1WAY_OFF &        // IOLOCK One-Way Set Enable bit (Unlimited Writes To RP Registers)
//    OSCIOFNC_ON &        // Primary Oscillator Output Function (OSCO functions as port I/O (RC15))
//    FCKSM_CSDCMD &       // Clock Switching and Monitor (Both Clock Switching and Fail-safe Clock Monitor are disabled)
//    FNOSC_FRCPLL &       // Oscillator Select (Fast RC oscillator with Postscaler and PLL module (FRCPLL))
//    IESO_ON              // Internal External Switch Over Mode (IESO mode (Two-speed start-up) enabled)
//);
int CONFIG1 __attribute__((space(prog), address(0x157FE))) = 0x3F6F;
//_CONFIG1(
//    WDTPS_PS32768 &      // Watchdog Timer Postscaler (1:32,768)
//    FWPSA_PR32 &         // WDT Prescaler (Prescaler ratio of 1:32)
//    WINDIS_OFF &         // Watchdog Timer Window (Standard Watchdog Timer is enabled,(Windowed-mode is disabled))
//    FWDTEN_OFF &         // Watchdog Timer Enable (Watchdog Timer is disabled)
//    ICS_PGx1 &           // Comm Channel Select (Emulator functions are shared with PGEC1/PGED1)
//    GWRP_OFF &           // General Code Segment Write Protect (Writes to program memory are allowed)
//    GCP_OFF &            // General Code Segment Code Protect (Code protection is disabled)
//    JTAGEN_OFF           // JTAG Port Enable (JTAG port is disabled)
//);

// p24fj128ga106.h (C:\Program Files (x86)\Microchip\mplabc30\v3.25\support\PIC24F\h) -  _CONFIG1 _CONFIG2, pic24fj128ga106.pdf
// Fosc = 10MHz*4(PLL) = 40MHz ; Fcy = Fosc/2 = 20MHz
// 10MHz - по неизвестной причине вместо заявленной 8MHz
// В случае изменения частоты Fcy пересчитать временные интервалы согласно файлам:
// 1 Timers - TimerRecalculation.xmcd (Mathcad)
// 2 Значения PR2 задающие скорость ШД - SpeedSM.xmcd, SpeedSM.xls
// 3 Проверить ШИМ уточнить напряжения соответствующие уровням мощьности в случае отличия изменить период OC1RS и пересчитать значения заполнения OC1R

unsigned char Reason = NO;

int main() {
    unsigned char i;

    InitPic();

//    _TRISE2 = 0;
//    _LATE2 = 1;
//
//    while (1){
//    }
    
    InitPMSettings();   // до включения прерываний и до инициализации модулей и датчиков!!! ( инициализация модулей требует загруженных настроек из PM)

    InitUart();
    InitTimers();       //инициализация таймеров - по окончании все таймеры Disable и Off
    InitADC();          //инициализация АЦП - по окончании Off

    SetInterruptPriority(); // установка приоритетов прерываний

    InitSM1150();
    SM1150_Set_Power();

    InitUS();

    EnableTimer2();
    InitStepMotor();// инициализация ШД перед инициализацией датчиков , т.к при инициализации датчиков проверяется состояние ШД в случае тревоги
    EnableTimer3();

    InitStepMotor1();// инициализация ШД перед инициализацией датчиков , т.к при инициализации датчиков проверяется состояние ШД в случае тревоги
    EnableTimer4();


    InitPneumatics();//после инициализации таймера3 и разрешения его работы

    InitGasPressureSensor();
    InitGasExtractSensor();
    InitGasPlasmaFlowSensor();
    InitGasTransportFlowSensor();

    InitPMTSensor();
    OnADC();
    
    EnableUart();
    EnableTimer1(); 
    EnableCnInt();
    



 
    while (1) { 
        if(MainPacketDelay == 0){
           //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
           //отправка основного пакета           
           //увеличить размер MAX_TX_DATA_SIZE в соответствии с максимальной длинной пакета.
            StartTxPacket(0, MAIN_PACKET);
            MoveDataToTxPacket (INT,PMTSensor.HeatingTimerSec);
            MoveDataToTxPacket (INT,PMTSensor.CurLevel);
            MoveDataToTxPacket (INT,SM1150.Power_Level);
            MoveDataToTxPacket (CHAR,SM1150.CurErrorState);
            MoveDataToTxPacket (CHAR,StepMotor.Trailer.Alarm); //ошибка шаговика
            MoveDataToTxPacket (CHAR,Reason);
            
            MoveDataToTxPacket (CHAR,StepMotor.State.Zero);
            MoveDataToTxPacket (CHAR,PMTSensor.HeatingReady);
            MoveDataToTxPacket (CHAR,PMTSensor.State);//? volatile
           
            MoveDataToTxPacket (CHAR,GasExtractSensor.CurErrorState);
            MoveDataToTxPacket (CHAR,GasPlasmaFlowSensor.CurErrorState);
            MoveDataToTxPacket (CHAR,GasPressureSensor.CurErrorState);
            MoveDataToTxPacket (CHAR,GasTransportFlowSensor.CurErrorState);

            MoveDataToTxPacket(INT,StepMotor.Presets.EndPhasePoint[ZERO]);
            MoveDataToTxPacket(INT, StepMotor.Presets.EndPhasePoint[FIRST] - StepMotor.Presets.EndPhasePoint[ZERO]);
            MoveDataToTxPacket(INT, StepMotor.Presets.EndPhasePoint[SECOND] - StepMotor.Presets.EndPhasePoint[FIRST]);
            MoveDataToTxPacket(INT,StepMotor.Presets.EndPhasePoint[THIRD] - StepMotor.Presets.EndPhasePoint[SECOND]);
            MoveDataToTxPacket(INT, StepMotor.Presets.EndPhasePoint[FOURTH] - StepMotor.Presets.EndPhasePoint[THIRD]);

            MoveDataToTxPacket(INT,StepMotor.Presets.PhaseSpeed[ZERO]);
            MoveDataToTxPacket(INT,StepMotor.Presets.PhaseSpeed[FIRST]);
            MoveDataToTxPacket(INT,StepMotor.Presets.PhaseSpeed[SECOND]);
            MoveDataToTxPacket(INT,StepMotor.Presets.PhaseSpeed[THIRD]);
            MoveDataToTxPacket(INT,StepMotor.Presets.PhaseSpeed[FOURTH]);

            MoveDataToTxPacket(CHAR,StepMotor.MovingSettings.Direction);
            MoveDataToTxPacket(INT, StepMotor.State.MovingStepCounter);
            MoveDataToTxPacket(CHAR,StepMotor.MovingSettings.MovingState);
            MoveDataToTxPacket(INT, StepMotor.MovingSettings.Speed);
            MoveDataToTxPacket(LONG,StepMotor.MovingSettings.TargetPosition);
            MoveDataToTxPacket(LONG,StepMotor.State.GeneralStepCounter);
            MoveDataToTxPacket(CHAR,StepMotor.State.WorkMode);

            MoveDataToTxPacket(CHAR,StepMotor.Trailer.Up);
            MoveDataToTxPacket(CHAR,StepMotor.Trailer.Down);

            MoveDataToTxPacket(INT,StepMotor.Presets.TransferCoefficientLV);
            MoveDataToTxPacket(INT,StepMotor.Presets.MaxSpeedOil);
            MoveDataToTxPacket(INT,StepMotor.Presets.ControlVolume);

            MoveDataToTxPacket(INT,StepMotor.DeveloperSettings.TransferCoefficientSL);
            MoveDataToTxPacket(INT,StepMotor.DeveloperSettings.MaxSpeed);
            MoveDataToTxPacket(INT,StepMotor.DeveloperSettings.MaxControlVolume);

            MoveDataToTxPacket (INT,Pneumatics.ValveInTime);
            MoveDataToTxPacket (INT,Pneumatics.ValveOutTime);
            MoveDataToTxPacket (INT,Pneumatics.NumberOfCycles);

            MoveDataToTxPacket (INT,PMTSensor.AverageNumberForPlasmaDetect);
            MoveDataToTxPacket (INT,PMTSensor.DecisionLevel);
            MoveDataToTxPacket (INT,PMTSensor.HeatingTime);
            //MoveDataToTxPacket (INT,PMTSensor.HeatingTimerSec);

            MoveDataToTxPacket (CHAR,SM1150.EnableState);
            MoveDataToTxPacket (CHAR,SM1150.WorkState);
            MoveDataToTxPacket (CHAR,Pneumatics.State);
            MoveDataToTxPacket (CHAR,PMTSensor.State);
            MoveDataToTxPacket (CHAR,PMTSensor.HeatingReady);

            MoveDataToTxPacket (CHAR,GasExtractSensor.CurErrorState);
            MoveDataToTxPacket (CHAR,GasPlasmaFlowSensor.CurErrorState);
            MoveDataToTxPacket (CHAR,GasPressureSensor.CurErrorState);
            MoveDataToTxPacket (CHAR,GasTransportFlowSensor.CurErrorState);

            
            MoveDataToTxPacket (CHAR,StepMotor.State.Block);
            MoveDataToTxPacket (CHAR,US.WorkState);
            MoveDataToTxPacket (CHAR,StepMotor.Trailer.UpBlock);
            MoveDataToTxPacket (CHAR,StepMotor.Trailer.DownBlock);
            MoveDataToTxPacket (CHAR,SM1150.Block);
            MoveDataToTxPacket (CHAR,PMTSensor.Block);   
            MoveDataToTxPacket (CHAR,GasExtractSensor.Block);
            MoveDataToTxPacket (CHAR,GasPlasmaFlowSensor.Block);
            MoveDataToTxPacket (CHAR,GasPressureSensor.Block);
            MoveDataToTxPacket (CHAR,GasTransportFlowSensor.Block);

            i=0;
            while(i<24){
                MoveDataToTxPacket (CHAR,SM1150.Dev_SM1150_Coeff[i]);
                i++;
            }

            MoveDataToTxPacket (INT,StepMotor1.State.GeneralStepCounter);
            MoveDataToTxPacket (INT,StepMotor1.State.MovingStepCounter);
            MoveDataToTxPacket (INT,StepMotor1.State.SavedPosition);
            MoveDataToTxPacket (INT,StepMotor1.MovingSettings.TargetPosition);
            MoveDataToTxPacket (CHAR,StepMotor1.MovingSettings.Direction);
            MoveDataToTxPacket (CHAR,StepMotor1.MovingSettings.MovingState);
            MoveDataToTxPacket (CHAR,StepMotor1.State.ZeroSetMarker);
            MoveDataToTxPacket (CHAR,StepMotor1.State.InSavedPositionMarker);           
            MoveDataToTxPacket (CHAR,StepMotor1.Trailer.State);


            MoveDataToTxPacket (INT,Uart.Rx.Buffer.RdIndex);
            MoveDataToTxPacket (INT,Uart.Rx.Buffer.WrIndex);
            MoveDataToTxPacket (INT,*Uart.Rx.Analyse.pntRxPacketByte);
            MoveDataToTxPacket (CHAR,Uart.Rx.Analyse.StartPacket);
            MoveDataToTxPacket (CHAR,U1RXREG);
            MoveDataToTxPacket (CHAR,Uart.Rx.Packet.Crc);
            MoveDataToTxPacket (CHAR,Uart.Rx.Analyse.CrcControl);


            EndTxPacket();
            SendTxPaket();
            MainPacketDelay = MAIN_PACKET_DALAY;
        }
        
        AnalyseRxBuffer();//анализирует на наличие пакета и в случае его обнаружения вызывает функцию RxPacketAnalysis() в которую собраны обработчики команд от каждого модуля
        CnReaction();//в данной функции собраны функции обработки прерываний всех датчиков использующих прерывания CN // обязательно должна стоять после функции ReadAndAnalyseRxPackets(), чтобы остановить выполнение команды решение о выполнении которой принято ошибочно в результате возникновения прерывания после соответствующей проверки в коде команды.
        StepReaction();//
        ADCCannelMeanCount();
        PMTSensorReaction();
        CheckModuleState();
    }
}//END main()
//------------------------------------------------------------------------------

void InitPic(void) {
    //настройки осцилятора
    CLKDIV = 0x0000; // Kpll = 1:1
    _DOZE = 0; //
    _DOZEN = 0; //
    _VREGS = 0; //
    _ALTIVT = 0; //
    //настройки аналоговых портов
    AD1PCFGL =0xFFFF;//все аналоговые порты как цифровые
}
//------------------------------------------------------------------------------
void AnalyseRxPacket(void) {//Добавить необходимый обработчик команд - данная функция вызывается функцией  ReadAndAnalyseRxPackets() в случае обнаружения пакета (без ошибок)
    CommandMemory();
    CommandStepMotor();
    CommandStepMotor1();
    CommandSM1150();
    CommandPneumatics();
    CommandUS();
    PMTSensorCommand();
    GasPressureSensorCommand();
    GasPlasmaFlowSensorCommand();
    GasTransportFlowSensorCommand();
    GasExtractSensorCommand();
    ReasonCommand();

}
void ReasonCommand(void){
    if(Uart.Rx.Packet.Command == RESET_REASON){
        Reason = NO;
    }
}
//------------------------------------------------------------------------------
void CheckModuleState(void){//функция обеспечивающая корректировку состояний в системе модуля реальному состоянию модуля (порта управления). Рассогласование может произойти по причине последовательности выполнения включения модуля(установка порта в состояние ON) и присвоения значения переменной системы модуля (State = ON) если прерывание возникает между этими операциями и выключает модуль.
    CheckSM1150WorkState();
    CheckUSWorkState();
    CheckPneumaticsState();
    CheckStepMotorState();
}
//void CommandBlock(void){//функция обработки команд установки блокировок по датчикам
//    if (Uart.Rx.Packet.Command == SET_BLOCKS) {
//        StartReadRxData();
//
//        ReadDataFromRxPacket(CHAR);
//        GasPressureSensor.Block =  Uart.Rx.RxPackDataToRead.Char[0];
//        if(GasPressureSensor.Block == YES){
//            GasPressureSensor.CurErrorState = SETTING_ALARM;
//        }
//
//        ReadDataFromRxPacket(CHAR);
//        GasExtractSensor.Block =  Uart.Rx.RxPackDataToRead.Char[0];
//        if(GasExtractSensor.Block == YES){
//            GasExtractSensor.CurErrorState = SETTING_ALARM;
//        }
//
//        ReadDataFromRxPacket(CHAR);
//        GasPlasmaFlowSensor.Block = Uart.Rx.RxPackDataToRead.Char[0];
//        if (GasPlasmaFlowSensor.Block == YES) {
//            DisableCnInt();
//            GasPlasmaFlowSensor.PrevPinState = GAS_PLASMA_NORMAL_PIN_STATE;
//            EnableCnInt();
//            if (GasPlasmaFlowSensor.CurErrorState != CHANGE) {//на случай если уже ждем обработку!
//                GasPlasmaFlowSensor.PrevErrorState = GasPlasmaFlowSensor.CurErrorState;
//                GasPlasmaFlowSensor.CurErrorState = CHANGE;
//            }
//        }
//
//        ReadDataFromRxPacket(CHAR);
//        GasTransportFlowSensor.Block = Uart.Rx.RxPackDataToRead.Char[0];
//        if (GasTransportFlowSensor.Block == YES) {
//            DisableCnInt();
//            GasTransportFlowSensor.PrevPinState = GAS_TRANSPORT_NORMAL_PIN_STATE;
//            EnableCnInt();
//            if (GasTransportFlowSensor.CurErrorState != CHANGE) {//на случай если уже ждем обработку!
//                GasTransportFlowSensor.PrevErrorState = GasTransportFlowSensor.CurErrorState;
//                GasTransportFlowSensor.CurErrorState = CHANGE;
//            }
//        }
//
//        ReadDataFromRxPacket(CHAR);
//        PMTSensor.Block = Uart.Rx.RxPackDataToRead.Char[0];
//        if(PMTSensor.Block == NO){
//            Pneumatics.NumberOfCycles = 1;
//        }else{
//
//        }
//    }
//}
//------------------------------------------------------------------------------

void __attribute__((interrupt, no_auto_psv)) _AddressError(void) {
    while(1){
    Putch(0x85);
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    VALVE_OUT = 1;}
    //asm("goto %0" : : "r"(USER_PROG_RESET));
}
//------------------------------------------------------------------------------

void __attribute__((interrupt, no_auto_psv)) _StackError(void) {
    while(1){
    Putch(0x86);
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    //asm("goto %0" : : "r"(USER_PROG_RESET));
    VALVE_OUT = 1;}

}
//------------------------------------------------------------------------------

void __attribute__((interrupt, no_auto_psv)) _MathError(void) {
    while(1){
    Putch(0x87);
    VALVE_OUT = 1;
    for (;;);}
    //asm("goto %0" : : "r"(USER_PROG_RESET));}

}
//------------------------------------------------------------------------------

void __attribute__((interrupt, no_auto_psv)) _OscillatorFail(void) {
    while(1){
    Putch(0xbb);
    VALVE_OUT = 1;
    for (;;);}
    //asm("goto %0" : : "r"(USER_PROG_RESET));
}
