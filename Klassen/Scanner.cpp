#include "Scanner.h"

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

void Scanner::executeCommand(int pcommand)
{
    switch (pcommand and 0x3F00)
    {
        case 0x3000:
        //MOVLW
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
        case 20 ... 23:
        //BSF
        case 
    }

}