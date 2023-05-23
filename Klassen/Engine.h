#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#ifndef Engine_H_
#define Engine_H_

class Engine
{
    private:

    public:
    Engine();
    ~Engine();
    int programmemory[1024]; //oberen 2 Bits sind immer 0, ggf. verunden
    void controlCommand();           // Interrupt, IP und Befehl aus Programmemory holen
    void executeCommand(int pCommand);
    int add(int pX, int pY);
    int DatamemoryB0[128];
    int DatamemoryB1[128];
    int PORTA,PORTB,TRISA,TRISB,FSR,TMR0,OPTION,PCL,STATUS,EEDATA,EEADR,EECON1,EECON2,PCLATH,INTCON;
    int zero, carry, Dcarry;
    int W;
    int IP;


} ;

#endif