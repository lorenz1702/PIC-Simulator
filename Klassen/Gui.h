#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#ifndef GUI_H_
#define GUI_H_

class Gui
{
    private:
    sf::Window* gui;

    public:
    Gui();
    ~Gui();

} ;

#endif