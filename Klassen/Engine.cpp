#include "Engine.h"

Engine::Engine()
{
    for(int i = 0; i < 1024; i++){
    this->programmemory[i] = 0;
    }
}

Engine::~Engine()
{
}