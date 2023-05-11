#include "Gui.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include <chrono>

Gui::Gui()
{
    //Create Window and Visuals
    window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "PIC-Simulator");
    window->setFramerateLimit(60);

    backgroundrect = new sf::RectangleShape(sf::Vector2f(1920,1080));
    backgroundrect->setFillColor(sf::Color(50,62,98,255));

    programbackgroundrect = new sf::RectangleShape(sf::Vector2f(500, 700));
    programbackgroundrect->setFillColor(sf::Color(255,255,255,255));
    programbackgroundrect->setPosition(sf::Vector2f(100,100));

    programmemoryrect = new sf::RectangleShape(sf::Vector2f(250,700));
    programmemoryrect->setFillColor(sf::Color(255,255,255,255));
    programmemoryrect->setPosition(sf::Vector2f(700,100));

    registerrect = new sf::RectangleShape(sf::Vector2f(250,450));
    registerrect->setFillColor(sf::Color(255,255,255,255));
    registerrect->setPosition(sf::Vector2f(1050,100));

    while(window->isOpen())
    {
        sf::Event event;

        while(window->pollEvent(event))                             //event handling loop
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                    window->close();
                    break;
                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::Escape)      //if Escape is pressed, close the window
                    {
                        window->close();                                       
                    }                                            
                
                break;
            }
        }

        window->clear();                                            //clear last frame

        window->draw(*backgroundrect);
        window->draw(*programbackgroundrect);    
        window->draw(*programmemoryrect);
        window->draw(*registerrect);                               //draw Visuals

        window->display();                                          //display updated frame
    }
}

Gui::~Gui()
{

}