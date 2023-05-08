#include "Engine.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include <iomanip>

using namespace std;

Engine::Engine()
{
    for(int i = 0; i < 1024; i++)
    {
        this->programmemory[i] = '0';
    }
}

Engine::~Engine()
{

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

int Engine::executeCommand(string pcommand)
{
    cout << "pcommand:" << pcommand << endl;
    int comm[4];
    comm[0] = stoi(pcommand.substr(0,1), nullptr, 16);
    comm[1] = stoi(pcommand.substr(1,1), nullptr, 16);
    comm[2] = stoi(pcommand.substr(2,1), nullptr, 16);
    comm[3] = stoi(pcommand.substr(3,1), nullptr, 16);

    cout << "comm[0]:" << comm[0] << endl;
    cout << "comm[1]:" << comm[1] << endl;
    cout << "comm[2]:" << comm[2] << endl;
    cout << "comm[3]:" << comm[3] << endl;
    
    switch (comm[0])
    {
        case 0:
        cout << "Case 0" << endl;
        switch(comm[1])
        {
            case 7:
            //ADDWF
            cout << "ADDWF" << endl;
            break;
            case 5:
            //ANDWF
            cout << "ANDWF" << endl;
            break;
            case 1:
            switch(comm[2])
            {
                case 8 ... 0xF:
                //CLRF
                cout << "CLRF" << endl;
                break;
                case 0:
                //CLRW
                cout << "CLRW" << endl;
                break;
            }
            case 9:
            //COMF
            cout << "COMF" << endl;
            break;
            case 3:
            //DECF
            cout << "DECF" << endl;
            break;
            case 0xB:
            //DECFSZ
            cout << "DECFSZ" << endl;
            break;
            case 0xA:
            //INCF
            cout << "INCF" << endl;
            break;
            case 0xF:
            //INCFSZ
            cout << "INCFSZ" << endl;
            break;
            case 4:
            //IORWF
            cout << "IORWF" << endl;
            break;
            case 8:
            //MOVF
            cout << "MOVF" << endl;
            break;
            case 0:
            switch(comm[2])
            {
                case 0:
                switch(comm[3])
                {
                    case 0:
                    //NOP
                    cout << "NOP" << endl;
                    break;
                    case 9:
                    //RETFIE
                    cout << "RETFIE" << endl;
                    break;
                }
                case 6:
                switch(comm[3])
                {
                    case 4:
                    //CLRWDT
                    cout << "CLRWDT" << endl;
                    break;
                    case 8:
                    //RETURN
                    cout << "RETURN" << endl;
                    break;
                    case 3:
                    //SLEEP
                    cout << "SLEEP" << endl;
                    break;
                }
                case 8 ... 0xF:
                //MOVWF
                cout << "MOVWF" << endl;
                break;
                
            }

            case 0xD:
            //RLF
            cout << "RLF" << endl;
            break;
            case 0xC:
            //RRF
            cout << "RRF" << endl;
            break;
            case 2:
            //SUBWF
            cout << "SUBWF" << endl;
            break;
            case 0xE:
            //SWAPF
            cout << "SWAPF" << endl;
            break;
            case 6:
            //XORWF
            cout << "XORWF" << endl;
            break;
        }
        break;
        case 1:

        cout << "Case 1" << endl;

        switch(comm[1])
        {
            case 0 ... 3:
            //BCF
            cout << "BCF" << endl;
            break;
            case 4 ... 7:
            //BSF
            cout << "BSF" << endl;
            break;
            case 8 ... 0xB:
            //BTFSC
            cout << "BTFSC" << endl;
            break;
            case 0xC ... 0xF:
            //BTFSS
            cout << "BTFSS" << endl;
            break;
        }
        break;
        case 2:

        cout << "Case 2" << endl;

        switch(comm[1])
        {
            case 0 ... 7:
            //CALL
            cout << "CALL" << endl;
            break;
            case 8 ... 0xF:
            //GOTO
            cout << "GOTO" << endl;
            break;
        }
        break;
        case 3:

        cout << "Case 3" << endl;
        
        switch(comm[1])
        {
            case 0xE:
            //ADDLW
            cout << "ADDLW" << endl;
            break;
            case 9:
            //ANDLW
            cout << "ANDLW" << endl;
            break;
            case 8:
            //IORLW
            cout << "IORLW" << endl;
            break;
            case 0:
            //MOVLW
            cout << "MOVLW" << endl;
            break;
            case 0xC:
            //SUBLW
            cout << "SUBLW" << endl;
            break;
            case 0xA:
            //XORLW
            cout << "XORLW" << endl;
            break;
        }
        break;
        default:
        ;
    }
    return 0;
}