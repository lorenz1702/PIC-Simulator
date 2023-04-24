#include "Scanner.h"
#include "Engine.h"


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
    std::ifstream file("TestProg_PicSim/TPicSim1.txt");
   if (!file.is_open()) {
        std::cout << "Could not open file " << "TestProg_PicSim/TPicSim1.txt" << std::endl;
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


