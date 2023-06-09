#pragma once

#ifndef Engine_H_
#define Engine_H_

#include <QMainWindow>

class Engine : public QObject
{
    Q_OBJECT

private:

    void RegisterHandlerBefore();
    void RegisterHandlerAfter(int intReg);
    void TimerHandler();
    void Prescaler();
    int CheckForInterrupt();
    int Interrupt();

public:
    Engine();
    ~Engine();
    int programmemory[1024]; //oberen 2 Bits sind immer 0, ggf. verunden
    void initializeEngine();
    void controlCommand();           // Interrupt, IP und Befehl aus Programmemory holen
    void executeCommand(int pCommand);
    int add(int pX, int pY);
    int Datamemory[2][128];
    int PORTA,PORTB,TRISA,TRISB,FSR,TMR0,OPTION,PCL,STATUS,EEDATA,EEADR,EECON1,EECON2,PCLATH,INTCON;
    int zero, carry, Dcarry;
    unsigned int W;
    int IP;
    int IPTemp;
    int RP0, GIE, statusTemp, statusTemp1, statusTemp2, T0CS;
    int RunTime;
    int WDT;
    int WDTE;
    int RB0;
    int RA4;


signals:
    void valueChanged();

} ;

#endif
