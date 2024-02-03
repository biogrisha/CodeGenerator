#pragma once
#include <string>
#include <vector>

enum class EElementType
{
	Condition,
	Action
};
struct SCodeElementCondition
{
	std::string Value;
	std::vector<std::string> Actions;
};


class CIfGenerator
{
public:
	void Generate(const std::string& Path);
private:

	void ParseFile(const std::string& Path);
	void Optimize();
	int Compare(std::vector<std::string> Values1,std::vector<std::string> Values2, int size);
	int FindFirstDifferentIndex(std::vector<SCodeElementCondition> Values1,std::vector<SCodeElementCondition> Values2);
	void DeleteEmptyLines();
	void PrintResultingCode();
	std::vector<std::vector<SCodeElementCondition>> ConditionsTable;
	std::vector<int> LastActionIndices;
};


