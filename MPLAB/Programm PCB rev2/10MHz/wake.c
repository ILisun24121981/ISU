#include "wake.h"

#include "uart.h"
#include "crc.h"
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
//void WakeData(unsigned char byte_number)
//
//Данные в Tx.Packet.Struct (см. UART.h ) располагаются в порядке
//соответствующем организации структурных единиц в Wake пакете:
//Address,Command,DataLength,Data[DataLength],Crc
//Функция Wake() доделывает данный пакет до полноценного Wake пакета согласно документации на Wake протокол и отправляет его в программный передающий буфер

void SendWakeTxByte(void){
    unsigned char temp;
    if(Uart.Tx.pntTxPacketByte == & Uart.Tx.Packet.Address){
        Uart.Tx.Packet.Crc = CRC8_calculate(WAKE_PACK,TX_PACK);
        WriteByteToTxBuffer(FEND);//начало Wake пакета
        temp = Uart.Tx.Packet.Address|0x80;//старший бит байта адреса в единицу
    }else
    if(Uart.Tx.pntTxPacketByte == & Uart.Tx.Packet.Command){
        temp = Uart.Tx.Packet.Command&0x7F;//старший бит байта команды в ноль
    }else{
        temp = *Uart.Tx.pntTxPacketByte;
    }
    StuffingSend(temp);//закоментировать чтобы отключить стаффинг
    return;
}//END WakeData()
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
void StuffingSend(unsigned char byte){
    if(byte == FEND){
       WriteByteToTxBuffer(FESC);
       WriteByteToTxBuffer(TFEND);
    }else
    if(byte == FESC){
       WriteByteToTxBuffer(FESC);
       WriteByteToTxBuffer(TFESC);
    }else{
       WriteByteToTxBuffer(byte);
    }
}//END Stuffing()

