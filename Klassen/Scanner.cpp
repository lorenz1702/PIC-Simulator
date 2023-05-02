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

int Scanner::putcommandsinprogrammemory(Engine &engine)
{
    
    //FILE* inputFile = fopen("TestProg_PicSim/TPicSim1.txt", "r");
    //std::ifstream inputFile("TestProg_PicSim/TPicSim1.txt");
    std::ifstream file("TestProg_PicSim/TPicSim1.LST");
    if (!file.is_open()) 
    {
        std::cout << "Could not open file " << "TestProg_PicSim/TPicSim1.LST" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Ignore lines before line 18
        if (line.find("                    ") == 0){
            printf("line not used: %s\n",line);
            continue;
        }

        // Get the index and value from the line
        printf("line: %s\n",line);

        std::string indexStr = line.substr(0, 4);
        std::string valueStr = line.substr(5, 4);
        
        printf("indexStr: %s\n",indexStr);
        printf("value: %s\n",valueStr);

        std::cout << indexStr << std::endl;
        std::cout << valueStr << std::endl;

        // Convert index and value to integer
        
        
        try {
            int index = std::stoi(indexStr);
            printf("index convert: %i\n",index);
            int value = std::stol(valueStr, nullptr, 16);
            printf("value convert: %i\n",value);


            if (index >= 0 && index < 1024) {
                engine.programmemory[index] = value;
            } else {
                std::cout << "Index out of range: " << index << std::endl;
            }
        } catch (std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
        
    }
    file.close();

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

void Scanner::executeCommand(std::string pcommand) //pcommand: for example 3011 -> second part of command
{
    //int test = 3011;
    //string s = to_string(pcommand);       // temporary string for appending "0x"
    pcommand.insert (0, "0x");
    std::cout << "pcommand:" + pcommand << "\n";
    int a = std::stoi(pcommand,nullptr,0) & 0x3FF0;
    string pnew = std::to_string(a);
    std::cout << "pnew:" + pnew << "\n";

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
