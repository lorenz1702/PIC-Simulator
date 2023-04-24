#include "Scanner.h"
#include "Engine.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>

using namespace std;

Scanner::Scanner()
{
}

Scanner::~Scanner()
{
}

int Scanner::putcommandsinprogrammemory(Engine engine)
{
    
    FILE* inputFile = fopen("TestProg_PicSim/TPicSim1.LST", "r");
    int fscan_return = 0;
    int buffer_programmemory [2];
    int counter = 0;
    
    while(fscanf(inputFile, "%4X %4X" ,&buffer_programmemory[0],&buffer_programmemory[1]) != EOF){
        printf("\n%4X,%4X\n",buffer_programmemory[0],buffer_programmemory[1]);
        engine.programmemory[counter][0] = buffer_programmemory[0];
        engine.programmemory[counter][1] = buffer_programmemory[1];
        //printf("Im Programmspeicher steht an der %x der Wert %x",engine.programmemory[i][0],engine.programmemory[i][1]);
        counter++;
    }
    return 0;
}

/*
    Commands needed:

    COMMAND:                    CODE:  
    ---------------------------------------
    MOVLW                       30
    ADDLW                       3E
    SUBLW                       3C
    CALL                        20          -> ks geben Adresse an -> wird an Programmzähler übergeben
    GOTO                        28          -> ks geben Adresse an -> wird an Programmzähler übergeben
    MOVWF                       00
    MOVF                        08
    SUBWF                       02
    DECFSZ                      0B 
    INCFSZ                      0F
    RLF                         0D
    RRF                         0C
    BSF                         14 - 17
    BCF                         10 - 13
    BTFSC                       18 - 1B
    BTFSS                       1C - 1F
    SLEEP                       0063
    ADDWF (PCL with PCLATH)     07


    Moreover:
    Interrupts
    Bytebefehle
    Timerfunktion
    EEPROM


*/

void Scanner::executeCommand(int pcommand) //pcommand: for example 3011 -> second part of command
{
    //int test = 3011;
    string s = to_string(pcommand);       // temporary string for appending "0x"
    s.insert (0, "0x");
    int a = stoi(s) & 0x3FF0;
    std::cout << stoi(s) << "\n";

    /*switch (pNewcommand)
    {
        case 0x3000:
        //MOVLW
        printf("hier");
        case 0x0700:
        //ADDLW
        case 60:
        //SUBLW
        case 32:
        //CALL
        case 40:
        //GOTO
        case 0:
        //MOVWF
        case 8:
        //MOVF
        case 2:
        //SUBWF
        case 11:
        //DECFSZ
        case 15:
        //INCFSZ
        case 13:
        //RLF
        case 12:
        //RRF
        //case 20 ... 23:
        //BSF
        //case 
        default:
        ;
    }
*/
}
