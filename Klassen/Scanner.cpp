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
    //ifstream inputFile("TestProg_PicSim/TPicSim1.txt");
    ifstream file("TestProg_PicSim/TPicSim1.LST");
    if (!file.is_open()) 
    {
        cout << "Could not open file " << "TestProg_PicSim/TPicSim1.LST" << endl;
        return 1;
    }

    string line;
    while (getline(file, line)) {
        // Ignore lines before line 18
        if (line.find("                    ") == 0){
            //printf("line not used: %s\n",line);
            continue;
        }

        // Get the index and value from the line
        //printf("line: %s\n",line);

        string indexStr = line.substr(0, 4);
        string valueStr = line.substr(5, 4);
        
        int index = stoi(indexStr);
        printf("index convert: %i\n",index);
        // Convert index and value to integer
        if (index >= 0 && index < 1024) {
            engine.programmemory[index] = valueStr;
        } else {
            cout << "Index out of range: " << index << endl;
        }
    }
    file.close();

    return 0;
    
    }


