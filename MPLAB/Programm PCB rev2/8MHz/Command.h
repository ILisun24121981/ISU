#ifndef _COMMAND_H
#define _COMMAND_H

    //_____________________________________________________________________________________
    //Принимаемые команды

    //Команда установки блокировок
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
     #define RESET_REASON                                     0x50
     
    //Команды ШД
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    #define SM_WRITE_DEV_SETTINGS                               0x00//записать настройки разработчика (для ШД)
    #define SM_SET_PRESETS_ADDITIONAL                           0x01//установить дополнительные текущие настройки (для ШД)
    #define SM_SET_PRESETS_PHASE                                0x02//установить текущие настройки Фаз анализа (для ШД)
    #define SM_WRITE_PRESETS_AS_DEFAULT                         0x03//записать дополнительные текущие настройки (для ШД)как дополнительные настройки по умолчанию
    #define SM_LOAD_DEFAULT_TO_PRESETS                          0x04//
    #define SM_SET_DOWN_TRAILER_BLOCK                           0x32//
    #define SM_SET_UP_TRAILER_BLOCK                             0x31//
    #define SM_SET_BLOCK                                        0x33//

    #define SM_SET_ZERO                 0x05//установить ноль в данной позиции (для ШД)
    #define SM_SET_WORK_MODE            0x06//перейти в рабочий режим (для ШД)
    #define SM_COLIBRATE_DONE           0x07
    #define SM_COLIBRATE_NOT_DONE       0x08
    #define SM_STOP                     0x09//остановить ШД (для ШД)
    #define SM_RETURN                   0x0A//выполнить возврат ШД (для ШД)
    #define SM_START_PHASES             0x0B//начать движение в соответствии с алгоритмом анализа (для ШД)
    #define SM_MOVE_BACK                0x0C//переместить ШД назад (для ШД)
    #define SM_MOVE_FORWARD             0x0D//переместить ШД вперед (для ШД)
    #define SM_WASHING                  0x0E//промывка (для ШД)

    #define SM1_DETERMINATE_POSITION                0x38
    #define SM1_SAVE_POSITION                       0x39
    #define SM1_MOVE_FORWARD                        0x3A
    #define SM1_MOVE_BACK                           0x3B
    #define SM1_STOP                                0x3C
    #define SM1_MOVE_TO_SAVED_POSITION              0x3D

    //Команды SM1150
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    #define SM1150_ENABLE_DISABLE               0x2A
    #define SM1150_ON                           0x23//начать/прекратить подачу мощности в волновод (для SM1150)
    #define SM1150_SET_POWER                    0x24//установить уровень мощности подаваемой в волновод (для SM1150)
    #define SM1150_SET_COEFF                    0x37
    #define SM1150_OFF                          0x20
    //Команды пневматический поджиг
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    #define PNEUMATICS_SET_SETTINGS           0x22//установить настройки (для пневматического поджига)
    #define PNEUMATICS_SET_FIRE               0x21//запустить процесс поджога плазмы

    #define PMT_SENSOR_SET_BLOCK              0x2B
    #define PMT_SENSOR_SET_DECISION_LEVEL     0x25
    #define PMT_SET_HEARTING_TIME             0x26
    #define PMT_STOP_HEARTING                 0x27
    #define PMT_SENSOR_SET_AVERAGE_NUMBER     0x30

    #define SAVE_FIRE_SETTINGS                0x28
    #define LOAD_FIRE_SETTINGS                0x29

    #define GAS_PRESSURE_SENSOR_SET_BLOCK          0x2E
    #define GAS_PLASMA_FLOW_SENSOR_SET_BLOCK       0x2D
    #define GAS_TRANSPORT_FLOW_SENSOR_SET_BLOCK    0x2F
    #define GAS_EXTRACT_SENSOR_SET_BLOCK           0x2C

    #define US_ON_OFF                              0x34
    #define EXPORT_PACKET                          0x35
    #define IMPORT_PACKET                          0x36
    //_____________________________________________________________________________________
    //Отправляемые команды
    //При использовании протокола Wake код команды занимает 7 !!! бит, что позволяет  передавать до 128 различных команд.(127=0x7F - последнее значение команды)
    #define MAIN_PACKET                         0x70
    
#endif //END #ifndef _COMMAND_H

