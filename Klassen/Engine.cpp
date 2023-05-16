#include "Engine.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>

using namespace std;

Engine::Engine()
{
    for(int i = 0; i < 1024; i++)
    {
        this->programmemory[i] = 0;
    }

    PORTA = 5;
    PORTB = 6;
    TRISA = 5;
    TRISB = 6;
    FSR = 4;
    TMR0 = 1;
    OPTION = 1;
    PCL = 2;
    STATUS = 3;
    EEDATA = 8;
    EEADR = 9;
    EECON1 = 8;
    EECON2 = 9;
    PCLATH = 10;
    INTCON = 11;

    IP = 0;
}

Engine::~Engine()
{

}

/*
    Commands needed:

    COMMAND:                    CODE:  
    -------------------------------------------------------
    ADDWF                       0700 - 07FF     00 0111 dfff ffff
    ANDWF                       0500 - 05FF     00 0101 dfff ffff
    CLRF                        0180 - 01FF     00 0001 1fff ffff
    CLRW                        0100            00 0001 0xxx xxxx
    COMF                        0900 - 09ff     00 1001 dfff ffff
    DECF                        0300 - 03ff     00 0011 dfff ffff
    DECFSZ                      0B00 - 0Bff     00 1011 dfff ffff
    INCF                        0A00 - 0Aff     00 1010 dfff ffff
    INCFSZ                      0F00 - 0Fff     00 1111 dfff ffff
    IORWF                       0400 - 04ff     00 0100 dfff ffff
    MOVF                        0800 - 08ff     00 1000 dfff ffff

    MOVWF                       0080 - 00FF     00 0000 1fff ffff
    NOP                         0000            00 0000 0xx0 0000
    
    RLF                         0D00 - 0Dff     00 1101 dfff ffff
    RRF                         0C00 - 0Cff     00 1100 dfff ffff
    SUBWF                       0200 - 02ff     00 0010 dfff ffff
    SWAPF                       0E00 - 0Eff     00 1110 dfff ffff
    XORWF                       0600 - 06ff     00 0110 dfff ffff

    BCF                         1000 - 13ff     01 00bb bfff ffff                         
    BSF                         1400 - 17ff     01 01bb bfff ffff
    BTFSC                       1800 - 1Bff     01 10bb bfff ffff
    BTFSS                       1C00 - 1Fff     01 11bb bfff ffff

    ADDLW                       3E00 - 3Eff     11 111x kkkk kkkk
    ANDLW                       3900 - 39ff     11 1001 kkkk kkkk
    CALL                        2000 - 27ff     10 0kkk kkkk kkkk   -> ks geben Adresse an -> wird an Programmzähler übergeben

    CLRWDT                      0064            00 0000 0110 0100

    GOTO                        2800 - 2Fff     10 1kkk kkkk kkkk  
    IORLW                       3800 - 38ff     11 1000 kkkk kkkk
    MOVLW                       3000 - 30ff     11 00xx kkkk kkkk

    RETFIE                      0009            00 0000 0000 1001
    RETURN                      0068            00 0000 0110 1000
    SLEEP                       0063            00 0000 0110 0011

    SUBLW                       3C00 - 3Cff     11 110x kkkk kkkk
    XORLW                       3A00 - 3Aff     11 1010 kkkk kkkk

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
void Engine::controlCommand()
{
    int currentcommand;
    currentcommand = programmemory[IP];
    executeCommand(currentcommand);

}

void Engine::executeCommand(int pCommand)   //pcommand: for example "3011" for MOVLW
{
    
    cout << "pCommand:" << pCommand << endl;
    int valueW, intTemp, result;
    valueW = W;
    //switch statement with all commands
    switch (pCommand)
    {
        case 0x0100:
        cout << "CLRW" << endl;
        break;
        case 0x0000:
        cout << "NOP" << endl;
        break;
        case 0x0064:
        cout << "CLRWDT" << endl;
        break;
        case 0x0009:
        cout << "RETFIE" << endl;
        break;
        case 0x0068:
        cout << "RETURN" << endl;
        break;
        case 0x0063:
        cout << "SLEEP" << endl;
        break;
    }

    int newCommand = pCommand & 0x3f00;
    cout << "NewCommand: " << newCommand << endl;

    switch(newCommand)
    {
        case 0x0700 ... 0x07FF:
        
        cout << "ADDWF" << endl;
        break;
        case 0x0500 ... 0x05FF:
        cout << "ANDWF" << endl;
        break;
        case 0x0180 ... 0x01FF:
        cout << "CLRF" << endl;
        break;
        case 0x0900 ... 0x09ff:
        cout << "COMF" << endl;
        break;
        case 0x0300 ... 0x03ff:
        cout << "DECF" << endl;
        break;
        case 0x0B00 ... 0x0Bff:
        cout << "DECFSZ" << endl;
        break;
        case 0x0A00 ... 0x0Aff:
        cout << "INCF" << endl;
        break;
        case 0x0F00 ... 0x0Fff:
        cout << "INCFSZ" << endl;
        break;
        case 0x0400 ... 0x04ff:
        cout << "IORWF" << endl;

        break;
        case 0x0800 ... 0x08ff:
        cout << "MOVF" << endl;
        break;
        case 0x0080 ... 0x00FF:
        cout << "MOVWF" << endl;
        break;
        case 0x0D00 ... 0x0Dff:
        cout << "RLF" << endl;
        break;
        case 0x0C00 ... 0x0Cff:
        cout << "RRF" << endl;
        break;
        case 0x0200 ... 0x02ff:
        cout << "SUBWF" << endl;
        break;
        case 0x0E00 ... 0x0Eff:
        cout << "SWAPF" << endl;
        break;
        case 0x0600 ... 0x06ff:
        cout << "XORWF" << endl;
        break;
        case 0x1000 ... 0x13ff:
        cout << "BCF" << endl;
        break;
        case 0x1400 ... 0x17ff:
        cout << "BSF" << endl;
        break;
        case 0x1800 ... 0x1Bff:
        cout << "BTFSC" << endl;
        break;
        case 0x1C00 ... 0x1Fff:
        cout << "BTFSS" << endl;
        break;
        case 0x2000 ... 0x27ff:
        cout << "CALL" << endl;
        break;
        case 0x2800 ... 0x2Fff:
        cout << "GOTO" << endl;
        break;
        case 0x3E00 ... 0x3Eff:
        cout << "ADDLW" << endl;
        cout << "pCommand: " << pCommand << endl;
        intTemp = pCommand & 0x01ff;
        cout << "intTemp: " << intTemp << endl;
        W = add(intTemp, valueW);
        cout << "W: " << W << endl; 
        break;
        case 0x3900 ... 0x39ff:
        cout << "ANDLW" << endl;
        intTemp = pCommand & 0x00ff;
        W = intTemp & valueW;  //the contents of W are bitwiseAND'ed with the literal intTemp 
        break;
        case 0x3800 ... 0x38ff:
        cout << "IORLW" << endl;  
        intTemp = pCommand & 0x00ff;
        W = intTemp | valueW;  //should not have influence on the Z-Flag
        break;
        case 0x3000 ... 0x30ff:
        cout << "MOVLW" << endl;
        intTemp = pCommand & 0x00ff;
        W = intTemp;
        break;
        case 0x3C00 ... 0x3Cff:
        cout << "SUBLW" << endl;
        intTemp = pCommand & 0x00ff;
        valueW = -W;
        W = intTemp + valueW;
        if (W <= 0){carry == 1;zero == 0;}
        if (W == 0)zero == 1;
        if (W > 0){carry == 0; zero == 0;}
        break;
        case 0x3A00 ... 0x3Aff:
        cout << "XORLW" << endl;
        intTemp = pCommand & 0x00ff;
        W = intTemp ^ valueW;
        break;
    }

    IP++;

    if(programmemory[IP] != 0)
    {
        
        controlCommand();
    }
    

}

int Engine::add(int pX, int pY)
{
    
    while (pY != 0)
    {
        carry = pX & pY;
        pX = pX ^ pY;
        pY = carry << 1;
    }
    return pX;
}
