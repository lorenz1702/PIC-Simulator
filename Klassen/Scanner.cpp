#include "Scanner.h"
#include "Engine.h"

Scanner::Scanner()
{
}

Scanner::~Scanner()
{
}

int Scanner::putcommandsinprogrammemory(Engine engine)
{
    
    FILE* inputFile = fopen("TestProg_PicSim/TPicSim1.txt", "r");
    int fscan_return = 0;
    int buffer_programmemory [2];
    int counter = 0;
    
    while(fscanf(inputFile, "%4X %4X" ,&buffer_programmemory[0],&buffer_programmemory[1]) != EOF){
        printf("\n%d,%d\n",buffer_programmemory[0],buffer_programmemory[1]);
        engine.programmemory[counter][0] = buffer_programmemory[0];
        engine.programmemory[counter][1] = buffer_programmemory[1];
        //printf("Im Programmspeicher steht an der %x der Wert %x",engine.programmemory[i][0],engine.programmemory[i][1]);
        counter++;
    }
  
    
    

    

    return 0;
}


