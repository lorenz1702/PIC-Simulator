#include "Gui.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include <chrono>

using namespace std;

Gui::Gui()
{
    //Create Window and Visuals
    window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "PIC-Simulator");
    window->setFramerateLimit(60);

    font.loadFromFile("C:\\Users\\sarah\\AppData\\Local\\Microsoft\\Windows\\Fonts\\SourceCodePro-Regular.ttf");

    ifstream file;
    file.open("TestProg_PicSim/TPicSim1.LST");
    stringstream buffer;
    buffer << file.rdbuf();
    string txt;
    txt = buffer.str();
    file.close();
    filetext.setString(txt);
    filetext.setColor(sf::Color::Black);
    filetext.setFont(font);
    filetext.setCharacterSize(16);
    filetext.setPosition(sf::Vector2f(105,105));

    backgroundrect = new sf::RectangleShape(sf::Vector2f(1920,1080));
    backgroundrect->setFillColor(sf::Color(50,62,98,255));

    programbackgroundrect = new sf::RectangleShape(sf::Vector2f(filetext.getLocalBounds().width + 10, filetext.getLocalBounds().height + 10));
    programbackgroundrect->setFillColor(sf::Color(255,255,255,255));
    programbackgroundrect->setPosition(sf::Vector2f(100,100));

    programmemoryrect = new sf::RectangleShape(sf::Vector2f(250,700));
    programmemoryrect->setFillColor(sf::Color(255,255,255,255));
    programmemoryrect->setPosition(sf::Vector2f(programbackgroundrect->getLocalBounds().width + 250, 100));

    registerrect = new sf::RectangleShape(sf::Vector2f(250,450));
    registerrect->setFillColor(sf::Color(255,255,255,255));
    registerrect->setPosition(sf::Vector2f(programmemoryrect->getPosition().x + 350, 100));


    float moveSpeed = 10000.0f;
    sf::Clock clock;

    while(window->isOpen())
    {
        clock.restart();
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

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            filetext.move(0 , moveSpeed * clock.getElapsedTime().asSeconds());
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            filetext.move(0, -moveSpeed * clock.getElapsedTime().asSeconds());
        }

        window->clear();                                            //clear last frame

        window->draw(*backgroundrect);
        window->draw(*programbackgroundrect);    
        window->draw(*programmemoryrect);
        window->draw(*registerrect);
        window->draw(filetext);                               //draw Visuals

        window->display();                                          //display updated frame
    }
}

Gui::~Gui()
{

}