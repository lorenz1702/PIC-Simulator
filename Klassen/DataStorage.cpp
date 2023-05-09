#include "DataStorage.h"


#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include <chrono>


DataStorage::DataStorage(int length, int height)
{
    rectangle.setSize(sf::Vector2f(length*0.2, height));
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(5);
    rectangle.setPosition(0, 0);
}

DataStorage::~DataStorage()
{

}
