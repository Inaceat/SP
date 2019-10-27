#include "stdafx.h"

#include "Tasks.hpp"


StdString GetCurrentWindowsDirectory()
{
	int currentDirectoryCharsCount = GetCurrentDirectory(0, NULL);
	auto currentDirectoryBuffer = new TCHAR[currentDirectoryCharsCount];

	GetCurrentDirectory(currentDirectoryCharsCount, currentDirectoryBuffer);

	StdString currentDirectoryPath(currentDirectoryBuffer, currentDirectoryBuffer + currentDirectoryCharsCount - 1);//-1 cause of \0

	delete[] currentDirectoryBuffer;

	return currentDirectoryPath;
}



void Task1::Do()
{
	StdString oldDirectory = GetCurrentWindowsDirectory();

	OUTPUT << "You are here :" << std::endl << "--->  " << oldDirectory << std::endl;


	OUTPUT << "And now.. WHOOOOOOSH.. And.." << std::endl;

	TCHAR* newDirectoryPath = const_cast<TCHAR*>(TEXT("C:\\Windows"));

	SetCurrentDirectory(newDirectoryPath);


	OUTPUT << "You are here :" << std::endl << "--->  " << GetCurrentWindowsDirectory() << std::endl;

	
	SetCurrentDirectory(oldDirectory.c_str());
}
