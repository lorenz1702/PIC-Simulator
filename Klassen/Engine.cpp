#include "Engine.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>

using namespace std;

void Engine::RegisterHandlerBefore()
{     
    //Programm Counter
    Datamemory[0][2] = IP;
    Datamemory[1][2] = IP;
    Datamemory[0][10]= (IP >> 8) & 0xFF;
    Datamemory[1][10]= (IP >> 8) & 0xFF;
    //STATUS and Bank Managment
    statusTemp = Datamemory[RP0][3] & 0x07;
    //INTCON Interrupt GIE
    GIE = ((Datamemory[1][11] >> 7) & 0x01)==1|((Datamemory[0][11] >> 7) & 0x01)==1;



}

void Engine::RegisterHandlerAfter()
{
    //Programm Counter
    if(intReg == 2){IP = Datamemory[RP0][intReg];cout << "Datamemory[RP0][intReg]: "<< Datamemory[RP0][intReg] << endl; intReg = 0;}
    //if(intReg == 10){IP = (Datamemory[RP0][10] << 8) | Datamemory[0][2];}
    //STATUS and Bank Managment
    RP0 = (Datamemory[RP0][3] >> 4) & 0x01;
    if (statusTemp != Datamemory[RP0][3] & 0x07){
        carry = Datamemory[RP0][3] & 0x01;
        zero = (Datamemory[RP0][3] >> 2) & 0x01;}
    //INTCON Interrupt GIE 
    /*
    Datamemory[RP0][11] = Datamemory[RP0][11] & 0x7f;
    Datamemory[RP0][11] = Datamemory[0][11] & 0x7f;
    Datamemory[RP0][11] = Datamemory[0][11] | (GIE << 7);
    Datamemory[RP0][11] = Datamemory[1][11] | (GIE << 7);
    */


    //TMR0
    T0CS = (Datamemory[1][1] >> 4) & 0x01;
    if(T0CS == 0){Datamemory[0][1]++;}

    if(Datamemory[0][1] == 255){Datamemory[0][1]=0; Datamemory[RP0][11] = Datamemory[RP0][11] | (1 << 2);}
}

int Engine::CheckForInterrupt()
{   
    int INTE = (Datamemory[RP0][11] >> 4) & 0x01;
    int RB0 = (Datamemory[0][6] & 0x01);
    int T0IE = (Datamemory[RP0][11] >> 5) & 0x01;
    int T0IF = (Datamemory[RP0][11] >> 2) & 0x01;
    int RBIE = (Datamemory[RP0][11] >> 3) & 0x01;
    int RBx = ((Datamemory[0][6] >> 4) & 0x0F) != 0;
    cout << "RBx: " << RBx << endl;
    cout << "T0IF: " << T0IF << endl;
    cout << "T0IE: " << T0IE << endl;
    cout << "GIE: " << GIE << endl;
    cout << "IP: "<< IP <<endl;

    

    if(GIE == 1){
        if(INTE == 1 && RB0 == 1){Interrupt();}
        if(RBIE == 1 && RBx == 1 ){Interrupt();}
        if(T0IE == 1 && T0IF == 1){Interrupt();}
    }
    
    return 0;
}

int Engine::Interrupt()
{
    IPTemp = IP;

    IP = 4;
    Datamemory[0][11] = Datamemory[0][11] & 0x7f;
    Datamemory[1][11] = Datamemory[1][11] & 0x7f;
    cout << "Interrupt at IP: "<< IPTemp << endl;
    return 0;
}

Engine::Engine() // Engine constructor
{
    //initialize programmemory
    for(int i = 0; i < 1024; i++)
    {
        this->programmemory[i] = 0;
    }
    for(int j = 0; j <= 1; j++){
    for(int i = 0; i < 128; i++)
    {
        this->Datamemory[j][i] = 0;
    }
    }
    //initialize speciel registers
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
    carry = 0;
    zero = 0;
    Dcarry = 0;
}

Engine::~Engine()                   //Engine destructor
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

void Engine::controlCommand()                   //set current command with help of programmemory and instruction pointer
{
    int currentcommand;
    currentcommand = programmemory[IP];
    
    executeCommand(currentcommand);

}

void Engine::executeCommand(int pCommand)       //execute Command handles given command and changes registers/values
{
    
    cout << "pCommand:" << pCommand << endl;
    int valueW, intTemp, intBit, result;
    valueW = W;

    RegisterHandlerBefore();
    cout << "Timer: " << Datamemory[0][1]<< endl;
    //Sleep(50);
    IP++;           //valueW stores value in W register
    //switch statement with all commands
    switch (pCommand)
    {
        case 0x0100:
        cout << "CLRW" << endl;
        W = 0;
        zero = 1;
        break;
        case 0x0000:
        cout << "NOP" << endl;
        break;
        case 0x0064:
        cout << "CLRWDT" << endl;
        break;
        case 0x0009:
        cout << "RETFIE" << endl;
        Datamemory[RP0][11] = Datamemory[0][11] | (1 << 7);
        Datamemory[RP0][11] = Datamemory[1][11] | (1 << 7);
        IP = IPTemp;
        break;
        case 0x0008:
        cout << "RETURN" << endl;
        IP = IPTemp;
        break;
        case 0x0063:
        cout << "SLEEP" << endl;
        break;
        case 0x0080 ... 0x00FF:
        cout << "MOVWF" << endl;
        intReg = pCommand & 0x007f;
        Datamemory[RP0][intReg] = valueW;
        break;
        case 0x0180 ... 0x01FF:
        cout << "CLRF" << endl;
        intReg = pCommand & 0x007f;
        Datamemory[RP0][intReg] = 0;
        zero=1;
        break;
    }

    int newCommand = pCommand & 0x3f00;
    cout << "NewCommand: " << newCommand << endl;

    switch(newCommand)
    {
        case 0x0700 ... 0x07FF:
        
        cout << "ADDWF" << endl;        // Addiere den Inhalt des W-Registers mit dem Inhalt des f-Registers
        intTemp = pCommand & 0x0080; // hol das Destination Bit
        cout << "Destination Bit: " << intTemp << endl;
        if(intTemp == 128)               // Wenn d = 1
        {
            intReg = pCommand & 0x007f;
            //aktuelle Bank auslagern ! Register mitgeben und Wert! Methode weiß selber, welche Bank gerade die aktuelle ist! + Methode zum ändern der aktuellen Bank
            Datamemory[RP0][intReg] = Datamemory[RP0][intReg] + valueW; // schreibs in das Register der aktuellen Bank
        } else 
        {
            intReg = pCommand & 0x007f;
            W = Datamemory[RP0][intReg] + valueW; // wenn es 0 ist, schreib das Ergebnis in W Register
        }

        break;
        case 0x0500 ... 0x05FF:
        cout << "ANDWF" << endl;
        intTemp = pCommand & 0x0080; // hol das Destination Bit
        cout << "Destination Bit: " << intTemp << endl;
        if(intTemp == 128)               // Wenn d = 1
        {
            intReg = pCommand & 0x007f;
            //aktuelle Bank auslagern ! Register mitgeben und Wert! Methode weiß selber, welche Bank gerade die aktuelle ist! + Methode zum ändern der aktuellen Bank
            Datamemory[RP0][intReg] = Datamemory[RP0][intReg] & valueW; // schreibs in das Register der aktuellen Bank
        } else 
        {
            intReg = pCommand & 0x007f;
            W = Datamemory[RP0][intReg] & valueW; // wenn es 0 ist, schreib das Ergebnis in W Register
        }
        break;
        case 0x0900 ... 0x09ff:
        cout << "COMF" << endl;
                intTemp = pCommand & 0x0080; // hol das Destination Bit
        cout << "Destination Bit: " << intTemp << endl;
        if(intTemp = 128)               // Wenn d = 1
        {
            intReg = pCommand & 0x007f;
            //aktuelle Bank auslagern ! Register mitgeben und Wert! Methode weiß selber, welche Bank gerade die aktuelle ist! + Methode zum ändern der aktuellen Bank
            Datamemory[RP0][intReg] = ~Datamemory[RP0][intReg] + ~valueW + 1; // schreibs in das Register der aktuellen Bank
        } else 
        {
            intReg = pCommand & 0x007f;
            W = ~Datamemory[RP0][intReg] + ~valueW + 1; // wenn es 0 ist, schreib das Ergebnis in W Register
        }
        break;
        case 0x0300 ... 0x03ff:
        cout << "DECF" << endl;
        intTemp = pCommand & 0x0080; // hol das Destination Bit
        cout << "Destination Bit: " << intTemp << endl;
        if(intTemp == 128)               // Wenn d = 1
        {
            intReg = pCommand & 0x007f;
            //aktuelle Bank auslagern ! Register mitgeben und Wert! Methode weiß selber, welche Bank gerade die aktuelle ist! + Methode zum ändern der aktuellen Bank
            cout << "Datamemory[RP0][intReg] before: " << Datamemory[RP0][intReg] << endl;
            intTemp = Datamemory[RP0][intReg] - 1; // schreibs in das Register der aktuellen Bank
            zero = (intTemp == 0) ? 1 :0;
            if(intTemp == -1){intTemp = 255;}
            Datamemory[RP0][intReg] = intTemp;
            cout << "Datamemory[RP0][intReg] after: " << Datamemory[RP0][intReg] << endl;
        } else 
        {
            intReg = pCommand & 0x007f;
            intTemp = Datamemory[RP0][intReg] - 1; // schreibs in das Register der aktuellen Bank
            intTemp = (intTemp == 0) ? 1 :0;
            if(intTemp == -1){intTemp = 255;}
            W = intTemp; // wenn es 0 ist, schreib das Ergebnis in W Register
        }
        break;
        case 0x0B00 ... 0x0Bff:
        cout << "DECFSZ" << endl;
        intTemp = pCommand & 0x0080; // hol das Destination Bit
        cout << "Destination Bit: " << intTemp << endl;
        if(intTemp == 128)               // Wenn d = 1
        {
            intReg = pCommand & 0x007f;
            //aktuelle Bank auslagern ! Register mitgeben und Wert! Methode weiß selber, welche Bank gerade die aktuelle ist! + Methode zum ändern der aktuellen Bank
            intTemp = Datamemory[RP0][intReg] - 1; // schreibs in das Register der aktuellen Bank
            zero = (intTemp == 0) ? 1 : 0;
            if(intTemp == 0){IP++;}
            if(intTemp == -1){intTemp = 255;}
            Datamemory[RP0][intReg] = intTemp;
            
        } else 
        {
            intReg = pCommand & 0x007f;
            //aktuelle Bank auslagern ! Register mitgeben und Wert! Methode weiß selber, welche Bank gerade die aktuelle ist! + Methode zum ändern der aktuellen Bank
            intTemp = Datamemory[RP0][intReg] - 1; // schreibs in das Register der aktuellen Bank
            zero = (intTemp == 0) ? 1 : 0;
            if(intTemp == 0){IP++;}
            if(intTemp == -1){intTemp = 255;}
            Datamemory[RP0][intReg] = intTemp;
        }
        break;
        case 0x0A00 ... 0x0Aff:
        cout << "INCF" << endl;
        intReg = pCommand & 0x007f;
        intTemp = Datamemory[RP0][intReg] + 1; // schreibs in das Register der aktuellen Bank
        if(intTemp == 256){intTemp = 0;}
        intTemp = (intTemp == 0) ? 1 :0;
        Datamemory[RP0][intReg] = intTemp;
        break;
        case 0x0F00 ... 0x0Fff:
        cout << "INCFSZ" << endl;
        intTemp = pCommand & 0x0080; // hol das Destination Bit
        cout << "Destination Bit: " << intTemp << endl;
        if(intTemp == 128)               // Wenn d = 1
        {
        intReg = pCommand & 0x007f;
        intTemp = Datamemory[RP0][intReg] + 1; // schreibs in das Register der aktuellen Bank
        if(intTemp <= 256){intTemp = 0; IP++;}
        intTemp = (intTemp == 0) ? 1 :0;
        Datamemory[RP0][intReg] = intTemp;
        } else {
        intReg = pCommand & 0x007f;
        intTemp = Datamemory[RP0][intReg] + 1; // schreibs in das Register der aktuellen Bank
        if(intTemp <= 256){intTemp = 0; IP++;}
        intTemp = (intTemp == 0) ? 1 :0;
        Datamemory[RP0][intReg] = intTemp;
        }
        break;
        case 0x0400 ... 0x04ff:
        cout << "IORWF" << endl;
        intTemp = pCommand & 0x0080; // hol das Destination Bit
        cout << "Destination Bit: " << intTemp << endl;
        if(intTemp == 128)               // Wenn d = 1
        {
            intReg = pCommand & 0x007f;
            //aktuelle Bank auslagern ! Register mitgeben und Wert! Methode weiß selber, welche Bank gerade die aktuelle ist! + Methode zum ändern der aktuellen Bank
            intTemp = Datamemory[RP0][intReg] | valueW; // schreibs in das Register der aktuellen Bank
            intTemp = (intTemp == 0) ? 1 :0;
            Datamemory[RP0][intReg] = intTemp;
        } else 
        {
            intReg = pCommand & 0x007f;
            intTemp = Datamemory[RP0][intReg] | valueW;; // schreibs in das Register der aktuellen Bank
            intTemp = (intTemp == 0) ? 1 :0;
            W = intTemp; // wenn es 0 ist, schreib das Ergebnis in W Register
        }
        break;
        case 0x0800 ... 0x08ff:
        cout << "MOVF" << endl;
        intTemp = pCommand & 0x0080; // hol das Destination Bit
        cout << "Destination Bit: " << intTemp << endl;
        if(intTemp == 128)               // Wenn d = 1
        {
            intReg = pCommand & 0x007f;
            //aktuelle Bank auslagern ! Register mitgeben und Wert! Methode weiß selber, welche Bank gerade die aktuelle ist! + Methode zum ändern der aktuellen Bank
            intTemp = Datamemory[RP0][intReg]; // schreibs in das Register der aktuellen Bank
            intTemp = (intTemp == 0) ? 1 :0;
            Datamemory[RP0][intReg] = intTemp;
        } else 
        {
            intReg = pCommand & 0x007f;
            intTemp = Datamemory[RP0][intReg]; // schreibs in das Register der aktuellen Bank
            intTemp = (intTemp == 0) ? 1 :0;
            W = intTemp; // wenn es 0 ist, schreib das Ergebnis in W Register
        }
        break;
        case 0x0D00 ... 0x0Dff:
        cout << "RLF" << endl;
        intTemp = pCommand & 0x0080; // hol das Destination Bit
        cout << "Destination Bit: " << intTemp << endl;
        if(intTemp == 128)               // Wenn d = 1
        {
            intReg = pCommand & 0x007f;
            intTemp = (Datamemory[RP0][intReg]<<1) | carry; //schreibs in das Register der aktuellen Bank
            carry = (Datamemory[RP0][intReg] & 0x0080) >> 7;
            cout << "carry bit:" << carry << endl;
            intTemp = intTemp & 0x00ff;
            Datamemory[RP0][intReg] = intTemp;
        } else 
        {
            intReg = pCommand & 0x007f;
            intTemp = (Datamemory[RP0][intReg]<<1) | carry; //schreibs in das Register der aktuellen Bank
            carry = (Datamemory[RP0][intReg] & 0x0000080) >> 31;
            intTemp = intTemp & 0x00ff;
            W = intTemp; // wenn es 0 ist, schreib das Ergebnis in W Register
        }
        cout << "Datamemory[RP0][intReg]: " << Datamemory[RP0][intReg] << endl;
        break;
        case 0x0C00 ... 0x0Cff:
        cout << "RRF" << endl;
        intTemp = pCommand & 0x0080; // hol das Destination Bit
        cout << "Destination Bit: " << intTemp << endl;
        if(intTemp == 128)               // Wenn d = 1
        {
            intReg = pCommand & 0x007f;
            intTemp = (Datamemory[RP0][intReg] >> 1) | (carry << 7); //schreibs in das Register der aktuellen Bank
            carry = Datamemory[RP0][intReg] & 1;
            intTemp = intTemp & 0x00ff;
            Datamemory[RP0][intReg] = intTemp;
        } else 
        {
            intReg = pCommand & 0x007f;
            intTemp = (Datamemory[RP0][intReg] >> 1) | (carry << 7); //schreibs in das Register der aktuellen Bank
            carry = Datamemory[RP0][intReg] & 1;
            intTemp = intTemp & 0x00ff;
            W = intTemp; // wenn es 0 ist, schreib das Ergebnis in W Register
        }
        cout << "Datamemory[RP0][intReg]: " << Datamemory[RP0][intReg] << endl;
        break;
        case 0x0200 ... 0x02ff:
        cout << "SUBWF" << endl;
        intTemp = pCommand & 0x0080; // hol das Destination Bit
        cout << "Destination Bit: " << intTemp << endl;
        if(intTemp == 128)               // Wenn d = 1
        {
            intReg = pCommand & 0x007f;
            //aktuelle Bank auslagern ! Register mitgeben und Wert! Methode weiß selber, welche Bank gerade die aktuelle ist! + Methode zum ändern der aktuellen Bank
            intTemp = ~valueW + 1;
            intTemp = Datamemory[RP0][intReg] + intTemp; // schreibs in das Register der aktuellen Bank
            carry = (result >= 0) ? 1 : 0;
            if(intTemp == 0){zero = 1; carry = 1;}
            Datamemory[RP0][intReg] = intTemp;
        } else 
        {
            intReg = pCommand & 0x007f;
            intTemp = ~valueW + 1;
            intTemp = Datamemory[RP0][intReg] + intTemp; // schreibs in das Register der aktuellen Bank
            carry = (result >= 0) ? 1 : 0;
            if(intTemp == 0){zero = 1; carry = 1;}
            W = intTemp; // wenn es 0 ist, schreib das Ergebnis in W Register
        }

        break;
        case 0x0E00 ... 0x0Eff:
        cout << "SWAPF" << endl;
        intTemp = pCommand & 0x0080; // hol das Destination Bit
        cout << "Destination Bit: " << intTemp << endl;
        if(intTemp == 128)               // Wenn d = 1
        {
            intReg = pCommand & 0x007f;
            unsigned int upperNibble = (Datamemory[RP0][intReg] & 0xFFFFFFF0) >> 4;  // Extract and shift upper nibble
            unsigned int lowerNibble = (Datamemory[RP0][intReg] & 0x0000000F) << 4;  // Extract and shift lower nibble
            intTemp = upperNibble | lowerNibble;  // Swap and combine nibbles
            Datamemory[RP0][intReg] = intTemp;
        } else 
        {
            intReg = pCommand & 0x007f;
            unsigned int upperNibble = (Datamemory[RP0][intReg] & 0xFFFFFFF0) >> 4;  // Extract and shift upper nibble
            unsigned int lowerNibble = (Datamemory[RP0][intReg] & 0x0000000F) << 4;  // Extract and shift lower nibble
            intTemp = upperNibble | lowerNibble;  // Swap and combine nibbles
            W = intTemp; // wenn es 0 ist, schreib das Ergebnis in W Register
        }
        cout << "Datamemory[RP0][intReg]: "<< Datamemory[RP0][intReg] <<endl;
        break;
        case 0x0600 ... 0x06ff:
        cout << "XORWF" << endl;

        intTemp = pCommand & 0x0080; // hol das Destination Bit
        cout << "Destination Bit: " << intTemp << endl;
        if(intTemp == 128)               // Wenn d = 1
        {
            intReg = pCommand & 0x007f;

            intTemp = Datamemory[RP0][intReg] ^ valueW;

            Datamemory[RP0][intReg] = intTemp;
        } else 
        {
            intReg = pCommand & 0x007f;
            intTemp = Datamemory[RP0][intReg] ^ valueW;
            W = intTemp; // wenn es 0 ist, schreib das Ergebnis in W Register
        }
        cout << "Datamemory[RP0][intReg]: "<< Datamemory[RP0][intReg] <<endl;
        break;
        case 0x1000 ... 0x13ff:
        cout << "BCF" << endl;
        //01 00bb bfff ffff
        cout << "pCommand: " << pCommand << endl;
        intBit = pCommand & 0x0380; // Maske für die b's
        intReg = pCommand & 0x007f; // Maske für das Register
        cout << "intBit: " << intBit << endl;
        // 0000 1000 0000 -> 80 080
        // 0001 1000 0000 -> 384 180
        // 0001 0000 0000 -> 256 100
        // 0010 0000 0000 -> 512 200
        // 0010 1000 0000 -> 320 280
        // 0011 0000 0000 -> 768 300
        // 0011 1000 0000 -> 896 380
        cout << "intReg: " << intReg << endl;
        if(Datamemory[RP0][3] >= 32) // Wenn das RP0 Bit des Status Registers der Bank 0 gesetzt ist
        {
            Datamemory[RP0][intReg] = intBit; // Änder den Wert in der Bank 1 (wird nochmal geändert werden müssen, da Funktion ausgelagert wird)
        } else 
        {
            Datamemory[RP0][intReg] = intBit;  // Wenn RP0 =  0 ist, dann ändere den Wert auf der Bank 0
            cout << "DatamemoryB0: " << Datamemory[RP0][intReg] << endl;
        }
        break;
        case 0x1400 ... 0x17ff:
        cout << "BSF" << endl;
        intBit = pCommand & 0x0380; // Maske für die b's
        intReg = pCommand & 0x007f; // Maske für das Register
        intBit = intBit >> 7;
        intTemp = 1 << intBit;
        Datamemory[RP0][intReg] = Datamemory[RP0][intReg] | intTemp;
        break;
        case 0x1800 ... 0x1Bff:
        cout << "BTFSC" << endl;
        intBit = pCommand & 0x0380; // Maske für die b's
        intReg = pCommand & 0x007f; // Maske für das Register
        intBit = intBit >> 7;
        intTemp = 1 << intBit;
        intTemp = (Datamemory[RP0][intReg] & intTemp) >> intBit;
        if(intTemp == 0){IP++;}
        break;
        case 0x1C00 ... 0x1Fff:
        cout << "BTFSS" << endl;
        intBit = pCommand & 0x0380; // Maske für die b's
        intReg = pCommand & 0x007f; // Maske für das Register
        intBit = intBit >> 7;
        intTemp = 1 << intBit;
        intTemp = (Datamemory[RP0][intReg] & intTemp) >> intBit;
        if(intTemp == 1){IP++;}
        break;
        case 0x2000 ... 0x27ff:
        cout << "CALL" << endl;
        IPTemp = IP + 1;
        intReg = pCommand & 0x07ff;
        cout << "intReg: " << intReg << endl;
        IP = intReg;
        break;
        case 0x2800 ... 0x2Fff:
        cout << "GOTO" << endl;
        intReg = pCommand & 0x07ff;
        //IPTemp = IP + 1;
        IP = intReg;
        break;
        case 0x3E00 ... 0x3Eff:
        cout << "ADDLW" << endl;
        cout << "pCommand: " << pCommand << endl;
        intTemp = pCommand & 0x01ff;
        cout << "intTemp: " << intTemp << endl;
        W = add(intTemp, valueW);
        break;
        case 0x3900 ... 0x39ff:
        cout << "ANDLW" << endl;
        intTemp = pCommand & 0x00ff;
        cout << "W vorher: " << W << endl; 
        W = intTemp & valueW;                 //the contents of W are bitwiseAND'ed with the literal intTemp 
        break;
        case 0x3800 ... 0x38ff:
        cout << "IORLW" << endl;  
        intTemp = pCommand & 0x00ff;
        cout << "W vorher: " << W << endl; 
        W = intTemp | valueW;
        cout << "W: " << W << endl;                    //should not have influence on the Z-Flag
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
        W = add(intTemp,valueW);
        cout << "W: " << W << endl; 
        
        if (W <= 0){carry = 0;zero = 0;}
        if (W == 0)zero = 1;
        if (W > 0){carry = 1; zero = 0;}
        
        break;
        case 0x3400 ... 0x37ff:
        cout << "RETLW" << endl;
        intTemp = pCommand & 0x00ff;
        W = intTemp;
        IP = IPTemp;

        break;
        case 0x3A00 ... 0x3Aff:
        cout << "XORLW" << endl;
        intTemp = pCommand & 0x00ff;
        cout << "W vorher: " << W << endl; 
        W = intTemp ^ valueW;
        cout << "W: " << W << endl; 
        break;
    }



    cout << "W: " << W << endl; 
    cout << "C: " << carry << endl;
    cout << "DC: " << Dcarry << endl;
    cout << "Z: " << zero << endl;  



    RegisterHandlerAfter();

    CheckForInterrupt();
    /*
    if(programmemory[IP] != 0)
    {
        
        controlCommand();
    }
    */
}

int Engine::add(int pX, int pY)
{
    
    while (pY != 0)
    {
        int carry1 = pX & pY;
        pX = pX ^ pY;
        pY = carry1 << 1;
    }
    return pX;
}
