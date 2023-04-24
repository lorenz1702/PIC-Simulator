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
    printf("hallowelt\n");
    engine.programmemory[1]=0;
    for (int i = 0; i < 10; i++) {
        //std::cout << "myArray[" << i << "] = " << (engine.programmemory[i]&0x3fff) << std::endl;

        printf("Wert Array%i\n",engine.programmemory[i]);

    }
    scanner.putcommandsinprogrammemory(engine);

    for (int i = 0; i < 10; i++) {
        //std::cout << "myArray[" << i << "] = " << (engine.programmemory[i]&0x3fff) << std::endl;
        printf("Wert Array%i\n",engine.programmemory[i]);
    }
}