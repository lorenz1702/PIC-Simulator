#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;

#ifndef Engine_H_
#define Engine_H_

class Engine
{
    private:
    

    public:
    Engine();
    ~Engine();
    string programmemory[1024]; //oberen 2 Bits sind immer 0, ggf. verunden

    int executeCommand(string pcommand);


};

#endif