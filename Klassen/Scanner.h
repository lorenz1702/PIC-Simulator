
#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Engine.h"

#ifndef Scanner_H_
#define Scanner_H_

class Scanner               //Ziffernstruktur: 0001 3011  0001-> Adresse, -> 30 = Befehlsindex , 11 =  Adressenargument (diesmal Ausnahme, kommt auf Tabelle an (mit k,b,x usw.))
{
    private:
    

    public:
    Scanner();
    ~Scanner();

    int putcommandsinprogrammemory(Engine &engine);
    void executeCommand(std::string pcommand);

} ;

#endif

