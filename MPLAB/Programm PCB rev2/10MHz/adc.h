#ifndef _ADC_H
#define _ADCT_H


#include "main.h"
//_____________________________________________________________________________________
// Определение констант

#define NUMBER_OF_ADC_CHANNELS  1         // 1-Температура,1-Датчик ФЭУ(горение плазмы) при добавлении канала изменить _SMPI и добавить соответствующие AD1PCFGLbits.PCFGx и AD1CSSLbits.CSSLx в InitADC();
#define SIZE_OF_ADC_BUFFER      500       // Размер буфера для каждого из каналов (максимальный размер усреднения - семплирования)

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Константы для наглядности кода
#define PMT_SENSOR_CHANNEL     0
#define TEMP_SENSOR_CHANNEL    1

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Конастанты настройки модуля ADC для работы   (задаем для инициализации в функции InitADC() )

#define AD1CON1_VAL         0x20E4          // 0010000011100100 - см. Комментарий - в конце файла
#define AD1CON2_VAL         0x0400          // 0000010000000000 - см. Комментарий - в конце файла 
#define AD1CON3_VAL         0x0C0E          // 0000110000001110 - см. Комментарий - в конце файла
#define AD1CHS_VAL          0x0000          // 0000000000000000 - см. Комментарий - в конце файла

//#define TEMPERATURE_POINTS 	 94
//_____________________________________________________________________________________
// Объявление переменных

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// определение типов
typedef struct{
    //unsigned char Block[NUMBER_OF_ADC_CHANNELS];
    unsigned short int PositionInBuffer; //позиция в буфере в которую записывается значение по прерыванию АЦП
    volatile unsigned short int Buffer[NUMBER_OF_ADC_CHANNELS][SIZE_OF_ADC_BUFFER];    //массив куда складываются значения оцифровки
    unsigned short int ChannelMean[NUMBER_OF_ADC_CHANNELS];                            //массив средних значений оцифровки по каждому из каналов за NUMBER_OF_ADC_SAMPLES измерений.
}ADC_t;

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// Объявление переменных
extern ADC_t ADC;
//extern const unsigned short int TemperatureTable[TEMPERATURE_POINTS + 1];
//_____________________________________________________________________________________
// Объявление функций
void InitADC(void);
void ADCCannelMeanCount(void);
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void);


//_____________________________________________________________________________________
// Макросы

#define ClearFlagADCInt()   IFS0bits.AD1IF = 0;     //очистить флаг прерывания по ADC
#define SetFlagADCInt()     IFS0bits.AD1IF = 1;     //установить флаг прерывания по Timer1
#define EnADCInt()          IEC0bits.AD1IE = 1;     //включить прерывания по ADC
#define DisADCInt()         IEC0bits.AD1IE = 0;     //выключить прерывания по ADC

#define OnADC()             ClearFlagADCInt();\
                            EnADCInt();\
                            AD1CON1bits.ADON = 1;\
                            AD1CON1bits.SAMP = 1;

#define OffADC()            DisADCInt();\
                            ClearFlagADCInt();\
                            AD1CON1bits.ADON = 0;\
                            AD1CON1bits.SAMP = 0;

#endif//END #ifndef _ADC_H

//Комментарии//_____________________________________________________________________________________
/*
 *  Коментарий AD1CON1_VAL//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //AD1CON1 //PIC24FJ256GA110 Family Data Sheet (REGISTER 21-1: AD1CON1: A/D CONTROL REGISTER 1)
 * AD1CON1.ADON             (POR=0)    = 0;    //A/D Converter is off
 * -U                       (POR=0)    = 0;
 * AD1CON1.ADSIDL           (POR=0)    = 1;    //Discontinue module operation when device enters Idle mode
 * -U                       (POR=0)    = 0;
 * -U                       (POR=0)    = 0;
 * -U                       (POR=0)    = 0;
 * AD1CON1.FORM1            (POR=0)    = 0;
 * AD1CON1.FORM0            (POR=0)    = 0;
    // AD1CON1.FORM<1:0> =0;                   //Integer (0000 00dd dddd dddd)
 * AD1CON1.SSRC2            (POR=0)    = 1;
 * AD1CON1.SSRC1            (POR=0)    = 1;
 * AD1CON1.SSRC0            (POR=0)    = 1;
    // AD1CON1.SSRC<2:0> =7;                   //Internal counter ends sampling and starts conversion (auto-convert)
 * -U                       (POR=0)    = 0;
 * -U                       (POR=0)    = 0;
 * AD1CON1.ASAM             (POR=0)    = 1;    //Sampling begins immediately after last conversion completes; SAMP bit is auto-set
 * AD1CON1.SAMP             (POR=0)    = 0;    //Hardware Clearable/Settable bit
 * AD1CON1.DONE             (POR=0)    = 0;    //Hardware Clearable/Settable bit
 *
*  Коментарий AD1CON2_VAL//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //AD1CON2 //PIC24FJ256GA110 Family Data Sheet (REGISTER 21-2: AD1CON2: A/D CONTROL REGISTER 2)
 * AD1CON2.VCFG2            (POR=0)    = 0;
 * AD1CON2.VCFG1            (POR=0)    = 0;
 * AD1CON2.VCFG0            (POR=0)    = 0;
    // AD1CON2.VCFG<2:0> =0;                   //0 - опорное AVdd AVss
 * r                        (POR=0)    = 0;    //r = Reserved bit
 * -U                       (POR=0)    = 0;
 * AD1CON2.CSCNA            (POR=0)    = 1;    //Scan inputs
 * -U                       (POR=0)    = 0;
 * -U                       (POR=0)    = 0;
 * AD1CON1.BUFS             (POR=0)    = 0;    //valid only when BUFM = 1
 * -U                       (POR=0)    = 0;
 * AD1CON1.SMPI3            (POR=0)    = 0;
 * AD1CON1.SMPI2            (POR=0)    = 0;
 * AD1CON1.SMPI1            (POR=0)    = 0;
 * AD1CON1.SMPI0            (POR=0)    = 0;
    // AD1CON2.SMPI<3:0> =0;                   //Sample/Convert Sequences Per Interrupt Selection bits - устанавливается в InitADC()!!!!
 * AD1CON1.BUFM             (POR=0)    = 0;    //Buffer configured as one 16-word buffer (ADC1BUFn<15:0>)
 * AD1CON1.ALTS             (POR=0)    = 0;    //Always uses MUX A input multiplexer settings
 *
*  Коментарий AD1CON3_VAL//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //AD1CON3 //PIC24FJ256GA110 Family Data Sheet (REGISTER 21-3: AD1CON3: A/D CONTROL REGISTER 3)
 * AD1CON3.ADRC             (POR=0)    = 0;    //Clock derived from system clock
 * r                        (POR=0)    = 0;    //r = Reserved bit
 * r                        (POR=0)    = 0;    //r = Reserved bit
 * AD1CON3.SAMC4            (POR=0)    = 0;
 * AD1CON3.SAMC3            (POR=0)    = 1;
 * AD1CON3.SAMC2            (POR=0)    = 1;
 * AD1CON3.SAMC1            (POR=0)    = 0;
 * AD1CON3.SAMC0            (POR=0)    = 0;    //Auto-Sample Time bits
    // AD1CON3.SAMC<4:0> =12;                   //Tsamp = 12TAD = 12*750ns = 9us
 * AD1CON3.ADCS7            (POR=0)    = 0;
 * AD1CON3.ADCS6            (POR=0)    = 0;
 * AD1CON3.ADCS5            (POR=0)    = 0;
 * AD1CON3.ADCS4            (POR=0)    = 0;
 * AD1CON3.ADCS3            (POR=0)    = 1;
 * AD1CON3.ADCS2            (POR=0)    = 1;
 * AD1CON3.ADCS1            (POR=0)    = 1;
 * AD1CON3.ADCS0            (POR=0)    = 0;    //Tcy = 50ns(Fcy = 20MHz)
    // AD1CON3.ADCS<7:0> =14;                   //TAD =15Tcy = 50*15ns = 750ns Tconv = 12TAD = 12*750ns = 9us   //minTAD=75ns
 //Время преобразования = Tsamp + Tconv = 18us (Fconv = 55.556 kHz)
*  Коментарий AD1CHS_VAL//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //AD1CHS //PIC24FJ256GA110 Family Data Sheet (REGISTER 21-4: AD1CHS: A/D INPUT SELECT REGISTER )
 * AD1CHS.CH0NB             (POR=0)    = 0;    //Channel 0 negative input is VR-
 * -U                       (POR=0)    = 0;
 * -U                       (POR=0)    = 0;
 * AD1CHS.CH0SB4            (POR=0)    = 0;
 * AD1CHS.CH0SB3            (POR=0)    = 0;
 * AD1CHS.CH0SB2            (POR=0)    = 0;
 * AD1CHS.CH0SB1            (POR=0)    = 0;
 * AD1CHS.CH0SB0            (POR=0)    = 0;    //Channel 0 positive input is AN0
    //AD1CHS.CH0SB<4:0> =0;                    //настройки MUXB не имеют значения так как AD1CON1.ALTS = 0
 * AD1CHS.CH0NA             (POR=0)    = 0;    //Channel 0 negative input is VR-
 * -U                       (POR=0)    = 0;
 * -U                       (POR=0)    = 0;
 * AD1CHS.CH0SA4            (POR=0)    = 0;
 * AD1CHS.CH0SA3            (POR=0)    = 0;
 * AD1CHS.CH0SA2            (POR=0)    = 0;
 * AD1CHS.CH0SA1            (POR=0)    = 0;
 * AD1CHS.CH0SA0            (POR=0)    = 0;    //Channel 0 positive input is AN0
    //AD1CHS.CH0SA<4:0> =0;                    //настройки CH0SA<4:0> MUXA не имеют значения так как AD1CON2.CSCNA = 1 , имеет значение только CH0NA
*/
