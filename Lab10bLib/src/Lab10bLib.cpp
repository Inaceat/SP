// Lab10bLib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


extern "C" __declspec(dllexport) std::string GetStringFromFile(std::string fileName, int positionInFile, int stringLength)
{
	//Get data from file
	HANDLE tempSourceFile = CreateFileA(fileName.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	SetFilePointer(tempSourceFile, positionInFile, nullptr, FILE_BEGIN);


	char* buffer = new char[stringLength];
	DWORD charsCount;

	ReadFile(tempSourceFile, buffer, stringLength, &charsCount, nullptr);


	std::string result(buffer, stringLength);

	//Clear
	CloseHandle(tempSourceFile);
	delete[] buffer;


	return result;
}