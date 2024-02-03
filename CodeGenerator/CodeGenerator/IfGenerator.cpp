#include "IfGenerator.h"
#include <fstream>
#include <iostream>
#include <numeric>

void CIfGenerator::Generate(const std::string& Path)
{
    ParseFile(Path);
    Optimize();
    DeleteEmptyLines();
    PrintResultingCode();
    //DEBUG
    //for(auto & CondLine : ConditionsTable)
    //{
    //    for(auto & Cond : CondLine)
    //    {
    //        std::cout<<Cond.Value<<" ";
    //        for(auto& ActionStr : Cond.Actions)
    //        {
    //            std::cout<<ActionStr<<";";
    //        }
    //    }
    //    std::cout<<std::endl;
    //}
}

void CIfGenerator::ParseFile(const std::string& Path)
{
    std::string line;
    const std::string ActionsStart = "-------What to do?--------";
    const std::string ActionsEnd = "---------------";
    // open a file in read mode.
    std::ifstream infile; 
    infile.open(Path.c_str()); 

    std::vector<SCodeElementCondition> ElementsLine;

    EElementType ElementType = EElementType::Condition;
    std::vector<SCodeElementCondition>  CondLine;
    std::vector<std::string>            ActionsLineTemp;
    while (std::getline(infile, line))
    {
        
        if( line == ActionsStart)//Action section started
        {
            ElementType = EElementType::Action;
        }else if( line == ActionsEnd)//Conditions section started
        {
            ElementType = EElementType::Condition;
            ConditionsTable.push_back(CondLine);
            ConditionsTable.back().back().Actions=ActionsLineTemp;
            CondLine.clear();
            ActionsLineTemp.clear();
        }else
        {
            if(ElementType == EElementType::Condition)
            {
                CondLine.push_back({line});
            }else if(ElementType == EElementType::Action)
            {
                ActionsLineTemp.push_back(line);
            }
        }
      
        
    }
    infile.close();
    
}

void CIfGenerator::Optimize()
{
    
    //Creating array with indices
    std::vector<int> CondIndices(ConditionsTable.size()); 
    std::iota (std::begin(CondIndices), std::end(CondIndices), 0);
    
    std::string CurrentCond;
    int HorizontalIndex = ConditionsTable[CondIndices[0]].size()-2;
    for(;HorizontalIndex>=0;HorizontalIndex--)
    {
        std::vector<int> CondIndicesTemp;
        //first condition vertically
        CurrentCond = ConditionsTable[CondIndices[0]][HorizontalIndex].Value;

        //size of first set of actions
        int IndexOfSameness = ConditionsTable[CondIndices[0]][HorizontalIndex+1].Actions.size();
        int LastVerticalIndex = CondIndices[0];
        for(int i = 1; i < CondIndices.size();i++)
        {
            int Ind = CondIndices[i];
            int IndPrev = CondIndices[i-1];
            
            if(ConditionsTable[Ind][HorizontalIndex].Value!=CurrentCond)
            {
                CurrentCond = ConditionsTable[Ind][HorizontalIndex].Value;
                
                CondIndicesTemp.push_back(IndPrev);

                if(IndexOfSameness>-1)
                {
                    int OffsetFromEnd = ConditionsTable[IndPrev][HorizontalIndex+1].Actions.size()-IndexOfSameness;
                    std::copy(ConditionsTable[IndPrev][HorizontalIndex+1].Actions.begin()
                        , ConditionsTable[IndPrev][HorizontalIndex+1].Actions.end()-OffsetFromEnd
                        , std::back_inserter(ConditionsTable[IndPrev][HorizontalIndex].Actions));

                    for(int j = i-1;;j--)
                    {
                        OffsetFromEnd = ConditionsTable[CondIndices[j]][HorizontalIndex+1].Actions.size()-IndexOfSameness;
                        ConditionsTable[CondIndices[j]][HorizontalIndex+1].Actions.erase(
                            ConditionsTable[CondIndices[j]][HorizontalIndex+1].Actions.begin()
                            , ConditionsTable[CondIndices[j]][HorizontalIndex+1].Actions.end()-OffsetFromEnd);

                        if(CondIndices[j]==LastVerticalIndex)
                        {
                            break;
                        }
                    
                    }
                }
                IndexOfSameness=ConditionsTable[Ind][HorizontalIndex+1].Actions.size();
                LastVerticalIndex = Ind;
                continue;
            }
            
            IndexOfSameness = Compare(ConditionsTable[Ind][HorizontalIndex+1].Actions
                ,ConditionsTable[IndPrev][HorizontalIndex+1].Actions
                ,IndexOfSameness);
            if(i == CondIndices.size()-1)
            {
                CurrentCond="";
                
                CondIndicesTemp.push_back(Ind);

                if(IndexOfSameness>-1)
                {
                    int OffsetFromEnd = ConditionsTable[Ind][HorizontalIndex+1].Actions.size()-IndexOfSameness;
                    std::copy(ConditionsTable[Ind][HorizontalIndex+1].Actions.begin()
                        , ConditionsTable[Ind][HorizontalIndex+1].Actions.end()-OffsetFromEnd
                        , std::back_inserter(ConditionsTable[Ind][HorizontalIndex].Actions));

                    for(int j = i;;j--)
                    {
                        OffsetFromEnd = ConditionsTable[CondIndices[j]][HorizontalIndex+1].Actions.size()-IndexOfSameness;
                        ConditionsTable[CondIndices[j]][HorizontalIndex+1].Actions.erase(
                            ConditionsTable[CondIndices[j]][HorizontalIndex+1].Actions.begin()
                            , ConditionsTable[CondIndices[j]][HorizontalIndex+1].Actions.end()-OffsetFromEnd);
                        
                        if(CondIndices[j]==LastVerticalIndex)
                        {
                            break;
                        }
                    }
                }
                continue;
            }
            
      
        }
        CondIndices = CondIndicesTemp;
    
    }

    
    
}

int CIfGenerator::Compare(std::vector<std::string> Values1,std::vector<std::string> Values2, int size)
{
    int MinSize = std::min(Values1.size(),Values2.size());
    MinSize = std::min(MinSize,size);
    for(int i = 0; i < MinSize;i++)
    {
        if(Values1[i] != Values2[i])
        {
            return i;
        }
        
    }
    return MinSize;
}

int CIfGenerator::FindFirstDifferentIndex(std::vector<SCodeElementCondition> Values1,
    std::vector<SCodeElementCondition> Values2)
{
    for(int i = 0; i<Values1.size();i++)
    {
        if(Values1[i].Value!=Values2[i].Value)
        {
            return i;
        }
    }
}


void CIfGenerator::DeleteEmptyLines()
{


    for (std::vector<std::vector<SCodeElementCondition>>::iterator it = ConditionsTable.begin(); 
         it!=ConditionsTable.end();)
    {
        bool IsEmpty = true;
        int i = 0;
        int LastActionIndex = 0;
        for(auto & Cond : *it)
        {
            if(!Cond.Actions.empty())
            {
                IsEmpty = false;
                LastActionIndex = i;
            }
            i++;
        }
        if(IsEmpty) 
            it = ConditionsTable.erase(it);
        else
        {
            LastActionIndices.push_back(LastActionIndex);
            ++it;
        }
    }
}

void CIfGenerator::PrintResultingCode()
{
    std::vector<std::string> Result;
    int OpenBraceLevel = 0;

    std::string IfStr = "if";

    for(int j = 0; j < LastActionIndices[0]+1;j++)
    {
        Result.push_back(IfStr+"("+ConditionsTable.back()[j].Value+")");
        Result.push_back("{");
        OpenBraceLevel++;
        for(std::string& Action : ConditionsTable.back()[j].Actions)
        {
            Result.push_back(Action);
        }
    }
    
    
    for(int i = ConditionsTable.size()-2;i>=0;i--)
    {
        
        int DiffIndex = FindFirstDifferentIndex(ConditionsTable[i],ConditionsTable[i+1]);
        //Compensate open braces
        for(int k = OpenBraceLevel;OpenBraceLevel>DiffIndex;OpenBraceLevel--)
        {
            Result.push_back("}");
        }
        for(int j = DiffIndex; j < LastActionIndices[i]+1;j++)
        {
            Result.push_back(IfStr+"("+ConditionsTable[i][j].Value+")");
            Result.push_back("{");
            OpenBraceLevel++;
            for(std::string& Action : ConditionsTable[i][j].Actions)
            {
                Result.push_back(Action);
            }
        }
    }
    //Compensate open braces
    for(int k = OpenBraceLevel;OpenBraceLevel>0;OpenBraceLevel--)
    {
        Result.push_back("}");
    }

    for(std::string& Line :Result)
    {
        std::cout<<Line<<std::endl;
    }
}
