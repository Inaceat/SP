#include "stdafx.h"

#include "Tasks.hpp"


void PrintCurrentDir()
{
	int currentDirectoryCharsCount = GetCurrentDirectory(0, NULL);
	auto currentDirectoryBuffer = new TCHAR[currentDirectoryCharsCount];


	GetCurrentDirectory(currentDirectoryCharsCount, currentDirectoryBuffer);


	std::string currentDirectoryPath(currentDirectoryBuffer, currentDirectoryBuffer + currentDirectoryCharsCount - 1);//-1 cause of \0

	std::cout << "You are here :" << std::endl << "--->  " << currentDirectoryPath << std::endl;


	delete[] currentDirectoryBuffer;
}


void Task1::Do()
{
	PrintCurrentDir();

	std::cout << "And now.. WHOOOOOOSH.. And.." << std::endl;

	TCHAR* newDirectoryPath = const_cast<TCHAR*>(TEXT("C:\\Windows"));

	SetCurrentDirectory(newDirectoryPath);

	PrintCurrentDir();
}
