#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "DataStorage.h"

#ifndef GUI_H_
#define GUI_H_

class Gui
{
    private:
    sf::RenderWindow* window;
    sf::RectangleShape* backgroundrect, *programbackgroundrect, *programmemoryrect, *registerrect;
    sf::Text filetext;
    sf::Font font;
    public:
    Gui();
    ~Gui();

} ;

#endif