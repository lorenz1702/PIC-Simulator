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
    std::string programmemory[1024]; //oberen 2 Bits sind immer 0, ggf. verunden
    void controlCommand();           // Interrupt, IP und Befehl aus Programmemory holen
    void executeCommand(std::string pcommand);
    int add(int x, int y);
    int DatamemoryB1[128];
    int compareDatamemoryB1[128];
    int PORTA,PORTB,TRISA,TRISB,FSR,TMR0,OPTION,PCL,STATUS,EEDATA,EEADR,EECON1,EECON2,PCLATH,INTCON;
    int W;
    int IP;


} ;

#endif