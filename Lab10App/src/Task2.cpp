#include "stdafx.h"


#include "Tasks.hpp"


__declspec(dllimport) std::string GetStringFromFile(std::string fileName, int positionInFile, int stringLength);


void Task2::Do()
{
	std::string fileName = "data/data.txt";
	int positionInFile = 12;
	int stringLength = 12;

	std::string result = GetStringFromFile(fileName, positionInFile, stringLength);

	std::cout << "String from file is \"" << result << "\"" << std::endl;
}
