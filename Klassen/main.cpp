#include "Gui.h"
#include "Scanner.h"
#include "Engine.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>

using namespace std;
//Programm starten:
//-> Im Terminal beim Pfad dieses Ordners "Klassen" mingw32-make eingeben


int bitwiseAND(int num1, int num2) {
    int result = num1 & num2;
    return result;
}
int main()
{
    //Gui Gui;


    Scanner scanner;

    Engine engine;

    scanner.putcommandsinprogrammemory(engine);
    engine.controlCommand();

    for (int i = 0; i < 15; i++) 
    {

        cout << "Wert Programmemory an der Stelle " << i << ": " << engine.programmemory[i] << endl;
    }
    

    
}