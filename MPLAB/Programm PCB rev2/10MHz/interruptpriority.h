#ifndef _INTERRUPTPRIORITY_H        
#define _INTERRUPTPRIORITY_H 


//_____________________________________________________________________________________
// Определение констант
    //Note
    /*
     * An interrupt source programmed to priority level 0 is effectively disabled,
     * since it can never be greater than the CPU priority.
     * The usable priority levels start at level 1 as the lowest priority and level 7 as the highest priority.
    */
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Константы Доп настройки (задаем !=0 если хотим использовать соответствующие функции контроллера)
    #define NSTDIS_VAL      0   //Использовать приоритеты назначенные в программе
                                //1 - отключить приоритеты
                                //В случае возникновения прерывании одновременно
                                //использовать приоритеты согласно расположению в таблице векторов (меньший адрес - больший приоритет)
    #define CPUPRIORITY_VAL 0   //Назначение приоритета программе
                                //Прерывания ниже данного приоритета не будут прерывать программу.
                                //All user interrupt sources can be disabled by setting IPL<2:0> = 111. -> CPUPRIORITY = 7
    //Value at POR: NSTDIS =0;CPUPRIORITY =0;
    //_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // Константы для задания приоритетов соответствующих прерываний
    #define	UTXIP_VAL   3	//Приоритет прерывания по передаче UART1
    #define	URXIP_VAL   7   //Приоритет прерывания по приему UART1
    #define     CNIP_VAL    7   //Приоритет прерыванией по выводам CN
    #define     T1IP_VAL    4   //Приоритет прерывания по Timer1 (1ms)
    #define     T2IP_VAL    5   //Приоритет прерывания по Timer2 (период задается при инициализации в функции InitPMSettings()) - влючен только во время работы ШД
    #define     T3IP_VAL    6   //Приоритет прерывания по Timer3 (10ms) - включен только во время работы клапанов
    #define     T4IP_VAL    6   //Приоритет прерывания по Timer4 (ms)
    #define     ADCIP_VAL   2
//_____________________________________________________________________________________
// Объявление функций
    void SetInterruptPriority(void);
#endif //END #ifndef _INTERRUPTPRIORITY_H

