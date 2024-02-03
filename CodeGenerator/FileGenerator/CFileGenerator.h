#pragma once
#include <string>
#include <vector>

class CFileGenerator
{
public:
    void Generate(const std::string& Path);


private:  
    std::vector<std::pair<std::string, std::vector<std::string>>> Params;
    std::vector<int> Mods;
    int Maxvalue = 0;

    //Helper function to generate mods array s.t. every number can be expressed as mod1*n_1+mod2*n_2...
    //Same as how we express numbers in the binary form 2^0*n_0+2^1*n_1+2^2*n_2+...
    void GenerateMods();

    //Generates all possible combinations for the parameters set
    void Print(void (*ValidationAndPrint)(std::vector<std::pair<std::string, std::string>>& YourParamCombination, int index,std::ofstream& Ofstream),std::ofstream& Ofstream);
};
