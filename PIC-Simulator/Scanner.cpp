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
    Engine engine;
}

Scanner::~Scanner()
{
}

int Scanner::putcommandsinprogrammemory(Engine &engine, std::string lstpath)
{

    ifstream file(lstpath);
    
    if (!file.is_open())                                        //error handling
    {
        cout << "Could not open file " << lstpath << endl;
        return 1;
    }

    string line;
    while (getline(file, line))
    {
        // Ignore lines before line 18
        if (line.find("                    ") == 0)
        {
            //printf("line not used: %s\n",line);
            continue;
        }

        // Get the index and value from the line
        //printf("line: %s\n",line);

        string indexStr = line.substr(0, 4);
        string valueStr = line.substr(5, 4);

        int index = stoi(indexStr, nullptr, 16);

        if (index >= 0 && index < 1024) {
                engine.programmemory[index] = stoi(valueStr, nullptr, 16);
            } else {
                cout << "Index out of range: " << index << endl;
            }

    }
    for (int i = 0; i < 30; i++) {
            cout << "Wert Programmemory an der Stelle " << i << ": " << engine.programmemory[i] << endl;
    }
    return 0;
}

/*
    Commands needed:

    COMMAND:                    CODE:  
    -------------------------------------------------------
    ADDWF                       07              00 0111 dfff ffff
    ANDWF                       05              00 0101 dfff ffff
    CLRF                        01              00 0001 1fff ffff
    CLRW                        01              00 0001 0xxx xxxx
    COMF                        09              00 1001 dfff ffff
    DECF                        03              00 0011 dfff ffff
    DECFSZ                      0B              00 1011 dfff ffff
    INCF                        0A              00 1010 dfff ffff
    INCFSZ                      0F              00 1111 dfff ffff
    IORWF                       04              00 0100 dfff ffff
    MOVF                        08              00 1000 dfff ffff

    MOVWF                       00              00 0000 1fff ffff
    NOP                         00              00 0000 0xx0 0000
    
    RLF                         0D              00 1101 dfff ffff
    RRF                         0C              00 1100 dfff ffff
    SUBWF                       02              00 0010 dfff ffff
    SWAPF                       0E              00 1110 dfff ffff
    XORWF                       06              00 0110 dfff ffff

    BCF                         10 - 13         01 00bb bfff ffff                         
    BSF                         14 - 17         01 01bb bfff ffff
    BTFSC                       18 - 1B         01 10bb bfff ffff
    BTFSS                       1C - 1F         01 11bb bfff ffff

    ADDLW                       3E              11 111x kkkk kkkk
    ANDLW                       39              11 1001 kkkk kkkk
    CALL                        20 - 27         10 0kkk kkkk kkkk   -> ks geben Adresse an -> wird an Programmzähler übergeben

    CLRWDT                      0064            00 0000 0110 0100

    GOTO                        28 - 2F         10 1kkk kkkk kkkk  
    IORLW                       38              11 1000 kkkk kkkk
    MOVLW                       30              11 00xx kkkk kkkk

    RETFIE                      0009            00 0000 0000 1001
    RETURN                      0068            00 0000 0110 1000
    SLEEP                       0063            00 0000 0110 0011

    SUBLW                       3C              11 110x kkkk kkkk
    XORLW                       3A              11 1010 kkkk kkkk

    f       Register file address (0x00 to 0x7F)
    W       Working register (accumulator)
    b       Bit address within an 8-bit file register
    k       Literal field, constant data or label
    x       Don’t care location (= 0 or 1)
            The assembler will generate code with x = 0. It is the
            recommended form of use for compatibility with all
            Microchip software tools.
    d       Destination select; d = 0: store result in W,
            d = 1: store result in file register f.
            Default is d = 1


    Moreover:
    Interrupts
    Bytebefehle
    Timerfunktion
    EEPROM


*/
