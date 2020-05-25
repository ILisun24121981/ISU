#include "packet.h"
//Функции формирования пакета на отправку. (см.

#include "uart.h"
#include "crc.h"

//_____________________________________________________________________________________
void StartTxPacket(unsigned char address,unsigned char command){//функция заполнения начальных структурных единиц пакета (Address,Command,DataLength) в структуре Uart.Tx.Packet.Struct
   Uart.Tx.Packet.Address = address;
   Uart.Tx.Packet.Command = command;
   Uart.Tx.Packet.DataLength = 0;
   Uart.Tx.pntTxPackData = & Uart.Tx.Packet.Data[0];
}
//_____________________________________________________________________________________
void MoveDataToTxPacket(unsigned char type, long data){//функция заполнения структурной единицы пакета Data в структуре Uart.Tx.Packet.Struct данными различных типов
    Long_t temp;
    temp.Long = data;
   //Порядок записи - старший байт числа -первым
#ifdef TX_HIGHT_BYTE_FIRST
   if(type == CHAR){
       *Uart.Tx.pntTxPackData = temp.Char[0];
       Uart.Tx.pntTxPackData++;
       Uart.Tx.Packet.DataLength++;
   }
   if(type == INT){ 
       *Uart.Tx.pntTxPackData= temp.Char[1];
       Uart.Tx.pntTxPackData++; 
       *(Uart.Tx.pntTxPackData) = temp.Char[0];
       Uart.Tx.pntTxPackData++;
       Uart.Tx.Packet.DataLength+=2;
   }
   if(type == LONG){
       *Uart.Tx.pntTxPackData= temp.Char[3];
       Uart.Tx.pntTxPackData++;
       *Uart.Tx.pntTxPackData= temp.Char[2];
       Uart.Tx.pntTxPackData++;
       *Uart.Tx.pntTxPackData= temp.Char[1];
       Uart.Tx.pntTxPackData++;
       *Uart.Tx.pntTxPackData= temp.Char[0];
       Uart.Tx.pntTxPackData++;
       Uart.Tx.Packet.DataLength+=4;
   }
#endif
   //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
#ifdef TX_LOW_BYTE_FIRST
   //Порядок записи - младший байт числа -первым
   if(type == CHAR){
       *Uart.Tx.pntTxPackData= temp.Char[0];
       Uart.Tx.pntTxPackData++;
       Uart.Tx.Packet.DataLength++;
   }
   if(type == INT){
       *Uart.Tx.pntTxPackData= temp.Char[0];
       Uart.Tx.pntTxPackData++;
       *Uart.Tx.pntTxPackData= temp.Char[1];
       Uart.Tx.pntTxPackData++;
       Uart.Tx.Packet.DataLength+=2;
   }
   if(type == LONG){
       *Uart.Tx.pntTxPackData= temp.Char[0];
       Uart.Tx.pntTxPackData++;
       *Uart.Tx.pntTxPackData= temp.Char[1];
       Uart.Tx.pntTxPackData++;
       *Uart.Tx.pntTxPackData= temp.Char[2];
       Uart.Tx.pntTxPackData++;
       *Uart.Tx.pntTxPackData= temp.Char[3];
       Uart.Tx.pntTxPackData++;
       Uart.Tx.Packet.DataLength+=4;
   }
#endif
}
//_____________________________________________________________________________________
void EndTxPacket(void){//функция заполнения конечной структурной единицы пакета - CRC в структуре Uart.Tx.Packet.Struct
    Uart.Tx.Packet.Crc = CRC8_calculate(SIMPLE_PACK,TX_PACK);
}
//_____________________________________________________________________________________
void ReadDataFromRxPacket(unsigned char type){//функция чтения данных из пришедшего пакета
#ifdef RX_HIGHT_BYTE_FIRST
    if(type == CHAR){
        Uart.Rx.RxPackDataToRead.Char[0] = Uart.Rx.Packet.Data[Uart.Rx.RxPackDataPointer];
        Uart.Rx.RxPackDataPointer++;   
    }
    if(type == INT){
        Uart.Rx.RxPackDataToRead.Char[1] = Uart.Rx.Packet.Data[Uart.Rx.RxPackDataPointer];
        Uart.Rx.RxPackDataPointer++;
        Uart.Rx.RxPackDataToRead.Char[0] = Uart.Rx.Packet.Data[Uart.Rx.RxPackDataPointer];
        Uart.Rx.RxPackDataPointer++;
    }
    if(type == LONG){
        Uart.Rx.RxPackDataToRead.Char[3] = Uart.Rx.Packet.Data[Uart.Rx.RxPackDataPointer];
        Uart.Rx.RxPackDataPointer++;
        Uart.Rx.RxPackDataToRead.Char[2] = Uart.Rx.Packet.Data[Uart.Rx.RxPackDataPointer];
        Uart.Rx.RxPackDataPointer++;
        Uart.Rx.RxPackDataToRead.Char[1] = Uart.Rx.Packet.Data[Uart.Rx.RxPackDataPointer];
        Uart.Rx.RxPackDataPointer++;
        Uart.Rx.RxPackDataToRead.Char[0] = Uart.Rx.Packet.Data[Uart.Rx.RxPackDataPointer];
        Uart.Rx.RxPackDataPointer++;

    }
#endif
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
#ifdef RX_LOW_BYTE_FIRST
        if(type == CHAR){
            Uart.Rx.RxPackDataToRead.Char[1] = Uart.Rx.Packet.Struct.Data[Uart.Rx.RxPackDataPointer];
            Uart.Rx.RxPackDataPointer++;
        }
        if(type == INT){
            Uart.Rx.RxPackDataToRead.Char[0] = Uart.Rx.Packet.Struct.Data[Uart.Rx.RxPackDataPointer];
            Uart.Rx.RxPackDataPointer++;
            Uart.Rx.RxPackDataToRead.Char[1] = Uart.Rx.Packet.Struct.Data[Uart.Rx.RxPackDataPointer];
            Uart.Rx.RxPackDataPointer++;
        }
        if(type == INT){
            Uart.Rx.RxPackDataToRead.Char[0] = Uart.Rx.Packet.Struct.Data[Uart.Rx.RxPackDataPointer];
            Uart.Rx.RxPackDataPointer++;
            Uart.Rx.RxPackDataToRead.Char[1] = Uart.Rx.Packet.Struct.Data[Uart.Rx.RxPackDataPointer];
            Uart.Rx.RxPackDataPointer++;
            Uart.Rx.RxPackDataToRead.Char[2] = Uart.Rx.Packet.Struct.Data[Uart.Rx.RxPackDataPointer];
            Uart.Rx.RxPackDataPointer++;
            Uart.Rx.RxPackDataToRead.Char[3] = Uart.Rx.Packet.Struct.Data[Uart.Rx.RxPackDataPointer];
            Uart.Rx.RxPackDataPointer++;
        }
#endif
}
//Комментарий//_____________________________________________________________________________________
/*
 * Пример формирования пакета на отправку
 * StartTxPacket(0,1);
    *  MoveDataToTxPacket (CHAR,0x1);
    *  MoveDataToTxPacket (CHAR,Data1);
    *  MoveDataToTxPacket (INT,Data2);
 * EndTxPacket();
 *
 * Пример считывания данных из принятого пакета
 *
 */
