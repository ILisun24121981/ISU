#include "US.h"

#include "uart.h"

US_t US;
//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Настроим порты управления US
//_____________________________________________________________________________________
void ConfigPortsUS(void) {
    US_ON_OFF_TRIS          = 0;
    US_ON_OFF_OD            = 0;
    US_ON_OFF_CNIE          = 0;
    US_ON_OFF_CNPUE         = 0;
    US_ON_OFF_CNPDE         = 0;
}
//_____________________________________________________________________________________
void InitUS(void){
    ConfigPortsUS();
    US.OffDelayCounterMSec = 0;
    US.OffDelayCounterSec = US_OFF_DELAY;
    US.OffMarker = OFF;
    US_Off();
}
//_____________________________________________________________________________________
void CheckUSWorkState(void){
    if(US_ON_OFF_PORT == 1){
        US.WorkState = ON;
    }else{
        US.WorkState = OFF;
    }
}
//_____________________________________________________________________________________
void CommandUS(void){
    if (Uart.Rx.Packet.Command == US_ON_OFF) {
        if(US.WorkState == ON){
            US_Off();
        }else{
            US_On();
        }
    }
}
