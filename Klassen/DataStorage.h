#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#ifndef DATASTORAGE_H_
#define DATASTORAGE_H_


class DataStorage
{

    public:
    sf::RectangleShape rectangle;
    DataStorage(int length, int height);
    ~DataStorage();
    

};


#endif