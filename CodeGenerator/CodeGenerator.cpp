// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <windows.h>

#include "CodeGenerator/IfGenerator.h"
#include "FileGenerator/CFileGenerator.h"

void FindCover(){std::cout <<"FindCover()"<<std::endl;}
void GetGun(){std::cout <<"GetGun()"<<std::endl;}
void Reload(){std::cout <<"Reload()"<<std::endl;}
void StartShooting(){std::cout <<"StartShooting()"<<std::endl;}
void StartHealing(){std::cout <<"StartHealing()"<<std::endl;}
void GetKnife(){std::cout <<"GetKnife()"<<std::endl;}
void Melee(){std::cout <<"Melee()"<<std::endl;}

enum class EAppMode
{
    GenerateFile,
    GenerateCode
};

int main()
{
    //To change conditions edit CFileGenerator::Generate function
    
    //Generate file path
    char* PathPtr;
    _get_pgmptr(&PathPtr);
    std::string Path =PathPtr;
    Path = Path.substr(0,Path.length()-17);
    
    Path+="ExecutionTable.txt";
       
    EAppMode AppMode = EAppMode::GenerateFile;

    switch (AppMode) {
    case EAppMode::GenerateFile:
        {
            CFileGenerator FileGenerator;
            FileGenerator.Generate(Path);
        }
        break;
    case EAppMode::GenerateCode:
        {
            CIfGenerator IfGenerator;
            IfGenerator.Generate(Path);
        }
        break;
    }
    

    

    int i = 1;
    int j = 2;
    int k = 2;
    int l = 2;
    
    enum class EWeaponType
    {
        Gun,
        Knife
    };
    EWeaponType WeaponType = EWeaponType::Gun;
    int AmmoCount = 9;
    bool HasEnemyNear = false;
    bool LowHP = false;

    
    

}

