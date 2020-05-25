#ifndef _PMT_SENSOR_H
#define _PMT_SENSOR_H

//_____________________________________________________________________________________
// ќпределение констант
#define DEFAULT_AVERAGE_NUMBER_FO_PLASMA_DETECT  50
#define DEFAULT_DECISION_LEVEL  10
#define DEFAULT_HEATING_TIME    10800
//_____________________________________________________________________________________
// ќбъ€вление переменных

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// определение типов
typedef struct{
    unsigned short int DefaultHeatingTime;
    unsigned short int HeatingTime;
    unsigned short int HeatingTimerMSec;
    volatile unsigned short int HeatingTimerSec;
    volatile unsigned char HeatingReady;

    volatile unsigned char Block;
    unsigned short int AverageNumberForPlasmaDetect;   // количество семплов ј÷ѕ по которым производитс€ усреднение дл€ прин€ти€ решени€ о горении плазмы
    unsigned short int DefaultAverageNumberForPlasmaDetect;
    unsigned short int CurLevel;        // текущий уровень (усредненый
    unsigned short int DecisionLevel;   // уровень прин€ти€ решени€ о горении плазиы (выше - горит, ниже - не горит)
    unsigned short int DefaultDecisionLevel;
    unsigned char State;
}PMTSensor_t;

//_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// ќбъ€вление переменных
extern PMTSensor_t PMTSensor;       //датчик состо€ни€ плазмы (горит/ не горит)

//_____________________________________________________________________________________
// ќбъ€вление функций
void InitPMTSensor(void);
void PMTSensorReaction(void);
void PMTSensorCommand(void);
#endif//END #ifndef _PMT_SENSOR_H
