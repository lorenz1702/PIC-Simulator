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
    
    gui->setFramerateLimit(60);

}

Gui::~Gui()
{

}