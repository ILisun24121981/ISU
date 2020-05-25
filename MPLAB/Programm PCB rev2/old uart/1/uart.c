#include "uart.h"

#include "wake.h"
#include "crc.h"

     Uart_t Uart;
      
//Все настройки инициализации InitUart() выставляются в файле uart.h !!!
//_____________________________________________________________________________________
void InitUart(void) {// функция инициализации UART модулей
    unsigned short int i;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Маппинг модулей
    __builtin_write_OSCCONL(OSCCON & 0xbf); //Unlock Sequence
        PPS_U1RX_REG = PPS_U1RX_PIN; // назначить PPS_U1RX_PIN входом PPS_U1RX_REG
        PPS_U1TX_PIN = PPS_U1TX_FUNC; // назначить PPS_U1TX_PIN функцию выхода PPS_U1TX_FUNC
    __builtin_write_OSCCONL(OSCCON | 0x40); //Lock Sequence
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //если в результате ремапинга используемые PR могут использоваться как аналоговые порты AN
    //назначим эти порты как цифровые
    #ifdef AN_U1TX
        AN_U1TX = 1; //как цифровой порт//PIC24FJ256GA110 Family Data Sheet//REGISTER 21-5: AD1PCFGL: A/D PORTCONFIGURATION REGISTER (LOW)//REGISTER 21-6: AD1PCFGH: A/D PORTCONFIGURATION REGISTER (HIGH)
    #endif                              
    #ifdef AN_U1RX
        AN_U1RX = 1;
    #endif 
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Настройка режима работы  
    U1MODE  = 0x0000;
    U1STA   = 0x0000;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //Настроим регистр U1MODE //PIC24FJ256GA110 Family Data Sheet (REGISTER 17-1: UxMODE: UARTx MODE REGISTER)
    U1MODE  = U1MODE_VAL;
    U1MODEbits.BRGH = BRGH_VAL;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //Настроим регистр U1STA  //PIC24FJ256GA110 Family Data Sheet (REGISTER 17-2: UxSTA: UARTx STATUS AND CONTROL REGISTER)
    U1STA   = U1STA_VAL;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //запишем значение в регистр скорости
    U1BRG   = U1BRG_VAL;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // настройка прерываний
    DisUartTxInt();
    DisUartRxInt();
    ClearFlagTxInt();
    ClearFlagTxInt();
    //Настройка приоритетов вынесена в отдельный файл interruptpriority.c
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Настройка начальных адресов приемного и передающего программных буферов
    Uart.Tx.Buffer.RdIndex   =0;
    Uart.Tx.Buffer.WrIndex   =0;
    Uart.Rx.Buffer.RdIndex   =0;
    Uart.Rx.Buffer.WrIndex   =0;

    i=0;
    while(i< TX_BUFSIZE){
        Uart.Tx.Buffer.Data[i]=0;
        i++;
    }

    i=0;
    while(i< RX_BUFSIZE){
        Uart.Rx.Buffer.Data[i]=0;
        i++;
    }

    Uart.Rx.StartRxPacketCount      = NO;
    Uart.Rx.StartRxPacketFilling    = NO;
}//END InitUart()
//_____________________________________________________________________________________
void EnableUart(void) {//функция включения модуля UART1

    EnUart();
    EnUartTx();            //Enable Transmit UART1 !!!causes a transmit interrupt two cycles after being set
    DELAY_105uS;
    ClearFlagTxInt();      //очистим флаг прерываний по передаче UART1
    ClearFlagRxInt();      //очистим флаг прерываний по приему UART1
    EnUartTxInt();         //включим прерывания по передаче UART1
    EnUartRxInt();         //включим прерывания по приему UART1
}//END EnableUart1()
//_____________________________________________________________________________________
void WriteByteToTxBuffer(unsigned char data) {//функция отправки одного байта
    if ((U1STAbits.UTXBF == 0) && (Uart.Tx.Buffer.RdIndex == Uart.Tx.Buffer.WrIndex)) { // есть место для хотя бы одного байта в аппаратном буфере и нет байт на отправку в программном
        U1TXREG = data; // отправляем байт в аппаратный передающий буфер
    } else {
        if(Uart.Tx.Buffer.WrIndex == (TX_BUFSIZE - 1)) { // индекс записи указывает последнюю ячейку в прог.буфере
            while (Uart.Tx.Buffer.RdIndex == 0) { // ждем когда освободится место
            }
        } else { // не в конце
            while ((Uart.Tx.Buffer.WrIndex + 1) == Uart.Tx.Buffer.RdIndex) { // ждем когда освободится место
            }
        }
        Uart.Tx.Buffer.Data[Uart.Tx.Buffer.WrIndex] = data;
        if (Uart.Tx.Buffer.WrIndex == (TX_BUFSIZE - 1)) {
            Uart.Tx.Buffer.WrIndex = 0;
        }else{
            Uart.Tx.Buffer.WrIndex++;
        }
    }
}//END WriteByteToTxBuffer()
//_____________________________________________________________________________________
void SendTxPaket(void){//функция передачи пакета из структуры Uart.Tx.Packet.Struct в программный передающий буфер.
    unsigned short int byte_number = ADDRESS_BYTE;
    while( byte_number <= CRC_BYTE_TX){
        #ifdef WAKE_TX_PACKET   //закоментировать в файле uart.h чтобы отключить Wake при передаче
            SendWakeTxPacket(byte_number);
        #else
            WriteByteToTxBuffer(Uart.Tx.Packet.Byte[byte_number]);
        #endif
        byte_number++;
        if(byte_number == DATA_BYTE + Uart.Tx.Packet.Struct.DataLength){
           byte_number = CRC_BYTE_TX;
        }
    }
}//END SendTxPaket()
//_____________________________________________________________________________________
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void) {
    ClearFlagTxInt();   
    if (Uart.Tx.Buffer.RdIndex != Uart.Tx.Buffer.WrIndex) { // есть данные на отправку в программном буфере
        U1TXREG = Uart.Tx.Buffer.Data[Uart.Tx.Buffer.RdIndex];
        if (Uart.Tx.Buffer.RdIndex == (TX_BUFSIZE-1)) {
            Uart.Tx.Buffer.RdIndex = 0;
        }else{
            Uart.Tx.Buffer.RdIndex++;
        }
    }
}//END _U1TXInterrupt(void)
//_____________________________________________________________________________________
void ReadAndAnalyseRxPackets(void){//функция считывания пакета из программного приемного буфера в структуру Uart.Rx.Packet.Struct
    static unsigned short int i = ADDRESS_BYTE;         //счетчик байт в принимаемом пакете
    static unsigned char fesc = NO;
    unsigned char crc=0;

    while (Uart.Rx.Buffer.RdIndex != Uart.Rx.Buffer.WrIndex) {//есть данные в приемном программном буфере        
        if (Uart.Rx.StartRxPacketFilling == NO){
            if(Uart.Rx.StartRxPacketCount != NO) {//не определено начало пакета, но она есть в программном буфере
                if (Uart.Rx.Buffer.Data[Uart.Rx.Buffer.RdIndex] != FEND) {
                } else {
                    Uart.Rx.Buffer.Data[Uart.Rx.Buffer.RdIndex] = 0;//стираем байт начала пакета
                    Uart.Rx.StartRxPacketFilling = YES; //нашли начало пакета
                    Uart.Rx.Packet.Struct.DataLength = 0;
                    Uart.Rx.StartRxPacketCount--;
                }
                Uart.Rx.Buffer.RdIndex++;
            }
//            }else{
//                Uart.Rx.Buffer.RdIndex = Uart.Rx.Buffer.WrIndex;
//            }
        }else{//если начало пакета определено начинаем формировать пакет в union Uart.Rx.Packet
            if(i == (DATA_BYTE + Uart.Rx.Packet.Struct.DataLength)){
                i = CRC_BYTE_RX;//запись в CRC
            }
            //_ _ _ _ _ _ _ _ _ _ _
            if (Uart.Rx.Buffer.Data[Uart.Rx.Buffer.RdIndex] == FEND) {//в данном пакете обнаружен байт начала пакета = ошибка приема
                Uart.Rx.StartRxPacketFilling = NO; //отмена формирования текущего пакета
                i = ADDRESS_BYTE;
                if(Uart.Rx.StartRxPacketCount == NO){
                    Uart.Rx.Buffer.Data[Uart.Rx.Buffer.RdIndex] = 0;//стираем байт начала пакета
                    Uart.Rx.Buffer.RdIndex = Uart.Rx.Buffer.WrIndex;
                }
            } else {
                if (fesc == 0){
                    if (Uart.Rx.Buffer.Data[Uart.Rx.Buffer.RdIndex] == FESC) {
                        fesc = 1;
                    }else{
                        Uart.Rx.Packet.Byte[i] = Uart.Rx.Buffer.Data[Uart.Rx.Buffer.RdIndex];
                        i++;
                    }
                } else {//предидущий считанный с буфера байт = FESC
                    if (Uart.Rx.Buffer.Data[Uart.Rx.Buffer.RdIndex] == TFEND) {
                        Uart.Rx.Packet.Byte[i] = FEND;
                        i++;
                        fesc = 0;
                    } else
                    if (Uart.Rx.Buffer.Data[Uart.Rx.Buffer.RdIndex] == TFESC) {
                        Uart.Rx.Packet.Byte[i] = FESC;
                        i++;
                        fesc = 0;
                    } else
                    if (Uart.Rx.Buffer.Data[Uart.Rx.Buffer.RdIndex] == FESC){//в данном пакете обнаружен байт FESC следующий за байтом FESC = ошибка приема
                        Uart.Rx.StartRxPacketFilling = NO; //отмена формирования текущего пакета
                        i = ADDRESS_BYTE;
                        fesc = 0;
                        if(Uart.Rx.StartRxPacketCount == NO){
                            Uart.Rx.Buffer.RdIndex = Uart.Rx.Buffer.WrIndex;
                        }
                    }
                }
                if(i > ADDRESS_BYTE){//записан байт адреса
                    Uart.Rx.Packet.Byte[ADDRESS_BYTE]&=0x7F;
                }
                if(i > CRC_BYTE_RX){//записан последний байт пакета - CRC_BYTE
                    crc = CRC8_calculate(WAKE_PACK,RX_PACK);
                    //Uart.Rx.StartRxPacketFilling = NO;//добавил
                    if(crc == Uart.Rx.Packet.Byte[CRC_BYTE_RX]){
                        Uart.Rx.StartRxPacketFilling = NO;
                        i = ADDRESS_BYTE;
                        RxPacketAnalysis();
                    }else{
                        Uart.Rx.StartRxPacketFilling = NO; //отмена формирования текущего пакета
                        i = ADDRESS_BYTE;
                    }
                }
                Uart.Rx.Buffer.RdIndex++;
            }
        }
    }
}//END ReadRxPaket()
//_____________________________________________________________________________________
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt (void){
    ClearFlagRxInt();
    if((Uart.Rx.StartRxPacketCount != NO)||(Uart.Rx.StartRxPacketFilling == YES)){//в приемном потоке обнаружено начало пакета
       Uart.Rx.Buffer.Data[Uart.Rx.Buffer.WrIndex] = U1RXREG;// отправляем принятый байт в приемный программный буфер
       if (Uart.Rx.Buffer.Data[Uart.Rx.Buffer.WrIndex] == FEND) {
           Uart.Rx.StartRxPacketCount++;
       }
       Uart.Rx.Buffer.WrIndex++;
        if (Uart.Rx.Buffer.WrIndex == RX_BUFSIZE) {
            Uart.Rx.Buffer.WrIndex = 0;
        }
    }else {
        if (U1RXREG == FEND) {
            Uart.Rx.Buffer.Data[Uart.Rx.Buffer.WrIndex] = FEND;
            Uart.Rx.StartRxPacketCount++;
            Uart.Rx.Buffer.WrIndex++;           
        } 
    }   
}//END _U1RXInterrupt ()
//_____________________________________________________________________________________
void Putch(unsigned char txbyte){//Функция передачи одного быйта по UART1 !!!Если не используются прерывания
    while(U1STAbits.UTXBF);     //U1STAbits.UTXBF = 1 = Transmit buffer is ful -> wait for FIFO spac
    U1TXREG = txbyte;           //put character onto UART FIFO to transmit
}//END PutCharUart()
//_____________________________________________________________________________________
unsigned char Getch(unsigned char *rxbyte){//Функция приема одного байта по UART1 !!!Если не используются прерывания
    while(!U1STAbits.URXDA);    //U2STAbits.URXDA = 0 = Receive buffer is empty ->//wait for  = 1= Receive buffer has data; at least one more character can be read;
    *rxbyte = U1RXREG;
}//END GetCharUart()
