
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Engine.h"
#include "qtmetamacros.h"

#ifndef Scanner_H_
#define Scanner_H_

class Scanner               //Ziffernstruktur: 0001 3011  0001-> Adresse, -> 30 = Befehlsindex , 11 =  Adressenargument (diesmal Ausnahme, kommt auf Tabelle an (mit k,b,x usw.))
{
private:


public:
    Scanner();
    ~Scanner();

    int putcommandsinprogrammemory(Engine &engine, std::string lstpath);

} ;

#endif

