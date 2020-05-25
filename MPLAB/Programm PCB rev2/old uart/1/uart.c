#include "uart.h"

#include "wake.h"
#include "crc.h"

     Uart_t Uart;
      
//��� ��������� ������������� InitUart() ������������ � ����� uart.h !!!
//_____________________________________________________________________________________
void InitUart(void) {// ������� ������������� UART �������
    unsigned short int i;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // ������� �������
    __builtin_write_OSCCONL(OSCCON & 0xbf); //Unlock Sequence
        PPS_U1RX_REG = PPS_U1RX_PIN; // ��������� PPS_U1RX_PIN ������ PPS_U1RX_REG
        PPS_U1TX_PIN = PPS_U1TX_FUNC; // ��������� PPS_U1TX_PIN ������� ������ PPS_U1TX_FUNC
    __builtin_write_OSCCONL(OSCCON | 0x40); //Lock Sequence
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //���� � ���������� ��������� ������������ PR ����� �������������� ��� ���������� ����� AN
    //�������� ��� ����� ��� ��������
    #ifdef AN_U1TX
        AN_U1TX = 1; //��� �������� ����//PIC24FJ256GA110 Family Data Sheet//REGISTER 21-5: AD1PCFGL: A/D PORTCONFIGURATION REGISTER (LOW)//REGISTER 21-6: AD1PCFGH: A/D PORTCONFIGURATION REGISTER (HIGH)
    #endif                              
    #ifdef AN_U1RX
        AN_U1RX = 1;
    #endif 
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // ��������� ������ ������  
    U1MODE  = 0x0000;
    U1STA   = 0x0000;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //�������� ������� U1MODE //PIC24FJ256GA110 Family Data Sheet (REGISTER 17-1: UxMODE: UARTx MODE REGISTER)
    U1MODE  = U1MODE_VAL;
    U1MODEbits.BRGH = BRGH_VAL;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //�������� ������� U1STA  //PIC24FJ256GA110 Family Data Sheet (REGISTER 17-2: UxSTA: UARTx STATUS AND CONTROL REGISTER)
    U1STA   = U1STA_VAL;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    //������� �������� � ������� ��������
    U1BRG   = U1BRG_VAL;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // ��������� ����������
    DisUartTxInt();
    DisUartRxInt();
    ClearFlagTxInt();
    ClearFlagTxInt();
    //��������� ����������� �������� � ��������� ���� interruptpriority.c
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // ��������� ��������� ������� ��������� � ����������� ����������� �������
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
void EnableUart(void) {//������� ��������� ������ UART1

    EnUart();
    EnUartTx();            //Enable Transmit UART1 !!!causes a transmit interrupt two cycles after being set
    DELAY_105uS;
    ClearFlagTxInt();      //������� ���� ���������� �� �������� UART1
    ClearFlagRxInt();      //������� ���� ���������� �� ������ UART1
    EnUartTxInt();         //������� ���������� �� �������� UART1
    EnUartRxInt();         //������� ���������� �� ������ UART1
}//END EnableUart1()
//_____________________________________________________________________________________
void WriteByteToTxBuffer(unsigned char data) {//������� �������� ������ �����
    if ((U1STAbits.UTXBF == 0) && (Uart.Tx.Buffer.RdIndex == Uart.Tx.Buffer.WrIndex)) { // ���� ����� ��� ���� �� ������ ����� � ���������� ������ � ��� ���� �� �������� � �����������
        U1TXREG = data; // ���������� ���� � ���������� ���������� �����
    } else {
        if(Uart.Tx.Buffer.WrIndex == (TX_BUFSIZE - 1)) { // ������ ������ ��������� ��������� ������ � ����.������
            while (Uart.Tx.Buffer.RdIndex == 0) { // ���� ����� ����������� �����
            }
        } else { // �� � �����
            while ((Uart.Tx.Buffer.WrIndex + 1) == Uart.Tx.Buffer.RdIndex) { // ���� ����� ����������� �����
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
void SendTxPaket(void){//������� �������� ������ �� ��������� Uart.Tx.Packet.Struct � ����������� ���������� �����.
    unsigned short int byte_number = ADDRESS_BYTE;
    while( byte_number <= CRC_BYTE_TX){
        #ifdef WAKE_TX_PACKET   //��������������� � ����� uart.h ����� ��������� Wake ��� ��������
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
    if (Uart.Tx.Buffer.RdIndex != Uart.Tx.Buffer.WrIndex) { // ���� ������ �� �������� � ����������� ������
        U1TXREG = Uart.Tx.Buffer.Data[Uart.Tx.Buffer.RdIndex];
        if (Uart.Tx.Buffer.RdIndex == (TX_BUFSIZE-1)) {
            Uart.Tx.Buffer.RdIndex = 0;
        }else{
            Uart.Tx.Buffer.RdIndex++;
        }
    }
}//END _U1TXInterrupt(void)
//_____________________________________________________________________________________
void ReadAndAnalyseRxPackets(void){//������� ���������� ������ �� ������������ ��������� ������ � ��������� Uart.Rx.Packet.Struct
    static unsigned short int i = ADDRESS_BYTE;         //������� ���� � ����������� ������
    static unsigned char fesc = NO;
    unsigned char crc=0;

    while (Uart.Rx.Buffer.RdIndex != Uart.Rx.Buffer.WrIndex) {//���� ������ � �������� ����������� ������        
        if (Uart.Rx.StartRxPacketFilling == NO){
            if(Uart.Rx.StartRxPacketCount != NO) {//�� ���������� ������ ������, �� ��� ���� � ����������� ������
                if (Uart.Rx.Buffer.Data[Uart.Rx.Buffer.RdIndex] != FEND) {
                } else {
                    Uart.Rx.Buffer.Data[Uart.Rx.Buffer.RdIndex] = 0;//������� ���� ������ ������
                    Uart.Rx.StartRxPacketFilling = YES; //����� ������ ������
                    Uart.Rx.Packet.Struct.DataLength = 0;
                    Uart.Rx.StartRxPacketCount--;
                }
                Uart.Rx.Buffer.RdIndex++;
            }
//            }else{
//                Uart.Rx.Buffer.RdIndex = Uart.Rx.Buffer.WrIndex;
//            }
        }else{//���� ������ ������ ���������� �������� ����������� ����� � union Uart.Rx.Packet
            if(i == (DATA_BYTE + Uart.Rx.Packet.Struct.DataLength)){
                i = CRC_BYTE_RX;//������ � CRC
            }
            //_ _ _ _ _ _ _ _ _ _ _
            if (Uart.Rx.Buffer.Data[Uart.Rx.Buffer.RdIndex] == FEND) {//� ������ ������ ��������� ���� ������ ������ = ������ ������
                Uart.Rx.StartRxPacketFilling = NO; //������ ������������ �������� ������
                i = ADDRESS_BYTE;
                if(Uart.Rx.StartRxPacketCount == NO){
                    Uart.Rx.Buffer.Data[Uart.Rx.Buffer.RdIndex] = 0;//������� ���� ������ ������
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
                } else {//���������� ��������� � ������ ���� = FESC
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
                    if (Uart.Rx.Buffer.Data[Uart.Rx.Buffer.RdIndex] == FESC){//� ������ ������ ��������� ���� FESC ��������� �� ������ FESC = ������ ������
                        Uart.Rx.StartRxPacketFilling = NO; //������ ������������ �������� ������
                        i = ADDRESS_BYTE;
                        fesc = 0;
                        if(Uart.Rx.StartRxPacketCount == NO){
                            Uart.Rx.Buffer.RdIndex = Uart.Rx.Buffer.WrIndex;
                        }
                    }
                }
                if(i > ADDRESS_BYTE){//������� ���� ������
                    Uart.Rx.Packet.Byte[ADDRESS_BYTE]&=0x7F;
                }
                if(i > CRC_BYTE_RX){//������� ��������� ���� ������ - CRC_BYTE
                    crc = CRC8_calculate(WAKE_PACK,RX_PACK);
                    //Uart.Rx.StartRxPacketFilling = NO;//�������
                    if(crc == Uart.Rx.Packet.Byte[CRC_BYTE_RX]){
                        Uart.Rx.StartRxPacketFilling = NO;
                        i = ADDRESS_BYTE;
                        RxPacketAnalysis();
                    }else{
                        Uart.Rx.StartRxPacketFilling = NO; //������ ������������ �������� ������
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
    if((Uart.Rx.StartRxPacketCount != NO)||(Uart.Rx.StartRxPacketFilling == YES)){//� �������� ������ ���������� ������ ������
       Uart.Rx.Buffer.Data[Uart.Rx.Buffer.WrIndex] = U1RXREG;// ���������� �������� ���� � �������� ����������� �����
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
void Putch(unsigned char txbyte){//������� �������� ������ ����� �� UART1 !!!���� �� ������������ ����������
    while(U1STAbits.UTXBF);     //U1STAbits.UTXBF = 1 = Transmit buffer is ful -> wait for FIFO spac
    U1TXREG = txbyte;           //put character onto UART FIFO to transmit
}//END PutCharUart()
//_____________________________________________________________________________________
unsigned char Getch(unsigned char *rxbyte){//������� ������ ������ ����� �� UART1 !!!���� �� ������������ ����������
    while(!U1STAbits.URXDA);    //U2STAbits.URXDA = 0 = Receive buffer is empty ->//wait for  = 1= Receive buffer has data; at least one more character can be read;
    *rxbyte = U1RXREG;
}//END GetCharUart()
