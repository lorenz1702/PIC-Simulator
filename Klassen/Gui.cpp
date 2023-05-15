#include "Gui.h"
#include "DataStorage.h"
#include "DataStorage.cpp"


#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include <chrono>

Gui::Gui()
{
    sf::RenderWindow gui;
    gui.create(sf::VideoMode::getDesktopMode(), "PIC-Simulator", sf::Style::Fullscreen);


    printf("length: %i\n", gui.getSize().x);
    printf("height: %i\n", gui.getSize().y);

    DataStorage datastorage(1920,1080);
    
    //IOPins iopins;
    //SpecialFunctionRegister specialfunctionregister;
    //ControlPanel controlpanel;
    //LSTData lstdata;

    gui.setFramerateLimit(60);

    while (gui.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (gui.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                gui.close();
        }

        gui.clear();
        gui.draw(datastorage.rectangle);
        gui.display();
    }
    


}

Gui::~Gui()
{

}