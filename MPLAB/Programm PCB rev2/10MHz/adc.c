#include "adc.h"
#include "PMTSensor.h"

ADC_t ADC;
//const unsigned int TemperatureTable[TEMPERATURE_POINTS + 1] =
//{
//966,933,901,869,839,810,781,754,726,700,
//675,651,627,604,581,559,539,518,499,480,
//461,444,426,410,393,378,363,349,335,322,
//309,296,285,273,262,252,241,231,222,213,
//205,196,188,181,174,166,160,153,147,141,
//136,130,126,121,116,112,108,104,100, 96,
// 93, 90, 86, 83, 81, 78, 76, 73, 71, 69,
// 67, 65, 63, 62, 60, 58, 57, 56, 55, 53,
// 52, 51, 50, 49, 48, 47, 46, 46, 45, 44,
// 44, 43, 43, 0  };

//_____________________________________________________________________________________
void InitADC(void){// функция инициализации ADC
    unsigned char i;
    unsigned short int j;

    AD1CON1 = 0x0000;
    AD1CON2 = 0x0000;
    AD1CON3 = 0x0000;
    AD1CHS  = 0x0000;
    AD1CSSL = 0x0000;

    AD1CON1 = AD1CON1_VAL;
    AD1CON2 = AD1CON2_VAL;
    _SMPI = NUMBER_OF_ADC_CHANNELS - 1;           //Interrupts at the completion of conversion for each 2nd sample/convert sequence (= количество каналов - 1 => для 3-х каналов =2 - изменить в случае добавления каналов)
    AD1CON3 = AD1CON3_VAL;
    AD1CHS  = AD1CHS_VAL;

//Настроим порты как аналоговые входы ( After a device Reset, all TRIS bits are set. и в InitPic() - AD1PCFGL =0xFFFF;//все аналоговые порты как цифровые )
    AD1PCFGLbits.PCFG6	=0;	//AN6	Сигнал ФЭУ
    //AD1PCFGLbits.PCFG14	=0;	//AN14	Температура

    AD1PCFGHbits.PCFG16 =0;     //Internal VBG/2 channel enabled for input scan
    AD1PCFGHbits.PCFG17 =0;     //Internal band gap (VBG) channel enabled for input scan

 //выбирем порты для сканирования
    AD1CSSLbits.CSSL6 = 1;     //AN6	Сигнал ФЭУ
    //AD1CSSLbits.CSSL14 = 1;     //AN14	Температура

    OffADC();

    i=0;
    j=0;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    for(i=0;i<NUMBER_OF_ADC_CHANNELS; i++){		// очистим буфер
        for(j=0;j<SIZE_OF_ADC_BUFFER; j++){
            ADC.Buffer[i][j] = 0;
        }
    }

    ADC.ChannelMean[PMT_SENSOR_CHANNEL] = 0;
    //ADC.ChannelMean[TEMP_SENSOR_CHANNEL] = 0;

    ADC.PositionInBuffer = 0;
}
//_____________________________________________________________________________________
void ADCCannelMeanCount(void){//функция вычисления среднего значения по каналу датчика ФЭУ за PMTSensor.AverageNumberForPlasmaDetect измерений
    unsigned short int i,j;
    unsigned long tempSum;
    unsigned short int tempPos;
    // получение среднего значения по каналу PMT

    DisADCInt();
    tempPos = ADC.PositionInBuffer;
    EnADCInt();

    i=0;
    if(tempPos < PMTSensor.AverageNumberForPlasmaDetect){
        i = SIZE_OF_ADC_BUFFER - PMTSensor.AverageNumberForPlasmaDetect + tempPos;
    }else{
        i = tempPos - PMTSensor.AverageNumberForPlasmaDetect;
    }
    tempSum = 0;
    j=0;
    while(j < PMTSensor.AverageNumberForPlasmaDetect){
        tempSum += ADC.Buffer[PMT_SENSOR_CHANNEL][i];
        if(i == SIZE_OF_ADC_BUFFER-1){
            i = 0;
        } else {
            i++;
        }
        j++;
    }
    ADC.ChannelMean[PMT_SENSOR_CHANNEL] = tempSum / PMTSensor.AverageNumberForPlasmaDetect;
}

//_____________________________________________________________________________________
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void){
// по прерыванию заполняем циклический программный буфер (на каждый канал NUMBER_OF_SAMPLES ячеек)

    ADC.Buffer[PMT_SENSOR_CHANNEL][ADC.PositionInBuffer]   = ADC1BUF0;
    //ADC.Buffer[TEMP_SENSOR_CHANNEL][ADC.PositionInBuffer]  = ADC1BUF1;
      
    if(ADC.PositionInBuffer == SIZE_OF_ADC_BUFFER-1){
        ADC.PositionInBuffer = 0;
    }else{
        ADC.PositionInBuffer++;
    }
    ClearFlagADCInt();
}
