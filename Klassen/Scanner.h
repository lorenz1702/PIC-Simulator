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

class Scanner
{
    private:

    
    

    public:
    Scanner();
    ~Scanner();

    int putcommandsinprogrammemory(Engine &engine);


} ;

#endif