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
    void executeCommand(std::string pcommand);

} ;

#endif