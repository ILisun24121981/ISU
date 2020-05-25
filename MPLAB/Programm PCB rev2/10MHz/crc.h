#ifndef _CRC_H
#define _CRC_H

//_____________________________________________________________________________________
//ќпределение констант
    #define SIMPLE_PACK         0   //CRC расчитываетс€ дл€ обычного пакета
    #define WAKE_PACK           1   //CRC расчитываетс€ дл€ Wake пакета

    #define TX_PACK             0   //CRC расчитываетс€ дл€ Tx пакета
    #define RX_PACK             1   //CRC расчитываетс€ дл€ Rx пакета

//_____________________________________________________________________________________
//ќбъ€вление переменных
    extern const unsigned char CRC_TABLE[256];

//_____________________________________________________________________________________
//ќпределение функций
    unsigned char CRC8_calculate(unsigned char type,unsigned char packet);
#endif//END #ifndef _CRC_H
