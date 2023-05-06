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

    rectangle = new sf::RectangleShape(sf::Vector2f(1920,1080));
    rectangle->setFillColor(sf::Color(50,62,98,255));


    
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

        window->draw(*rectangle);                                   //draw Visuals

        window->display();                                          //display updated frame
    }
}

Gui::~Gui()
{

}