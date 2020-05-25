#ifndef _PACKET_H
#define _PACKET_H

#include "uart.h"
//_____________________________________________________________________________________
//ќпределение констант
    //указатель типа переменной при записи данных в фомируемый на отправку пакет (функци€ MoveDataToTxPacket())
    #define CHAR            0
    #define INT             1
    #define LONG            2
//_____________________________________________________________________________________
//ќбъ€вление функций
    void StartTxPacket(unsigned char address,unsigned char command);
    void MoveDataToTxPacket(unsigned char type, long data);
    void EndTxPacket(void);
    void ReadDataFromRxPacket(unsigned char type);
//_____________________________________________________________________________________
// ќпределение макросов
    #define StartReadRxData()     Uart.Rx.RxPackDataPointer = 0;
#endif//END #ifndef _TXPACK_H

