#include "Gui.h"
#include "Scanner.h"
#include "Engine.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>


//Programm starten:
//-> Im Terminal beim Pfad dieses Ordners "Klassen" mingw32-make eingeben

int main()
{
    //Gui Gui;

    Scanner scanner;

    Engine engine;
    printf("hallowelt");

    scanner.putcommandsinprogrammemory(engine);
}