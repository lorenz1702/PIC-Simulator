#include "Gui.h"


#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include <chrono>

Gui::Gui()
{
    gui = new sf::Window(sf::VideoMode::getDesktopMode(), "PIC-Simulator", sf::Style::Fullscreen);
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
    gui->setFramerateLimit(60);

}

Gui::~Gui()
{

}