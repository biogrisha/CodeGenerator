#include "CFileGenerator.h"
#include <fstream>
#include <iostream>

void CFileGenerator::Generate(const std::string& Path)
{
    //Write your parameters and actions
    std::vector<std::string> WeaponType         = { " == EWeaponType::Gun"," == EWeaponType::Knife" };
    std::vector<std::string> AmmoCount          =  { " > 10"," < 10" };
    std::vector<std::string> HasEnemyNear       =  { "== true","== false"};
    std::vector<std::string> LowHP              =  { "== true","== false"};
    
    //Put them here in your order
    
    Params.emplace_back("WeaponType",WeaponType  );
    Params.emplace_back("AmmoCount",AmmoCount   );
    Params.emplace_back("HasEnemyNear",HasEnemyNear);
    Params.emplace_back("LowHP",LowHP       );
    
    GenerateMods();

    // open a file in write mode.
    std::ofstream outfile;
    outfile.open(Path.c_str());

    //Write them in the file
    //Don't touch this part below
    Print([](std::vector<std::pair<std::string, std::string>>& YourParamCombination, int index,std::ofstream& Ofstream)
    {
        for(int i = 0; i < YourParamCombination.size();i++)
        {
            Ofstream <<YourParamCombination[i].first<< YourParamCombination[i].second<<std::endl;
        }
        Ofstream<<"-------What to do?--------"<<std::endl;
        Ofstream<<"---------------"<<std::endl;
    }
    ,outfile);
    outfile.close();
    system(("start " + Path).c_str());
}

void CFileGenerator::GenerateMods()
{
    std::reverse(Params.begin(), Params.end());
    int Value = 1;
    Mods.push_back(1);
    for (auto& Param : Params)
    {
        Value *= Param.second.size();
        Mods.push_back(Value);
    }
    Maxvalue = Value;
}

void CFileGenerator::Print(
    void(* ValidationAndPrint)(std::vector<std::pair<std::string, std::string>>& YourParamCombination, int index,std::ofstream& Ofstream),std::ofstream& Ofstream)
{
    std::vector<std::pair<std::string, std::string>>Combination;
    Combination.resize(Params.size());
    for (int i = 0; i < Maxvalue; i++)
    {
        int remainder = i;
        for (int j = 0; j < Params.size(); j++)
        {
            int ModsInd = Params.size() - 1 - j;
            int ParamInd = Params.size() - 1 - j;
            int index = remainder / Mods[ModsInd];
            remainder = remainder - index * Mods[ModsInd];
            Combination[j].first = Params[ParamInd].first;
            Combination[j].second = Params[ParamInd].second[index];

        }
        ValidationAndPrint(Combination, i,Ofstream);
    }
}
