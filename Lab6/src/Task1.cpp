#include "stdafx.h"

#include "Tasks.hpp"
#include <minwinbase.h>
#include <minwinbase.h>


std::wstring GetCurrentWindowsDirectory()
{
	int currentDirectoryCharsCount = GetCurrentDirectory(0, NULL);
	auto currentDirectoryBuffer = new TCHAR[currentDirectoryCharsCount];

	GetCurrentDirectory(currentDirectoryCharsCount, currentDirectoryBuffer);

	std::wstring currentDirectoryPath(currentDirectoryBuffer, currentDirectoryBuffer + currentDirectoryCharsCount - 1);//-1 cause of \0

	delete[] currentDirectoryBuffer;

	return currentDirectoryPath;
}


void Task1::Do()
{
	//Read 4 bytes, starting from 9th
	int offsetBytes = 8;
	int bufferBytes = 4;


	//Get current directory
	std::wstring startingDirectory = GetCurrentWindowsDirectory();


	//Paths
	std::wstring tempDirectory = startingDirectory + L"/data/temp";
	std::wstring tempFilePath = tempDirectory + L"/tempFile";
	std::wstring answerFilePath = startingDirectory + L"/data/answer.txt";
	std::wstring sourceFilePath = startingDirectory + L"/data/datafile";


	//Create temp copy
	CreateDirectory(tempDirectory.c_str(), NULL);
	
	CopyFile(sourceFilePath.c_str(), tempFilePath.c_str(), FALSE);


	//Get data from tmp copy
	HANDLE tempSourceFile = CreateFile(tempFilePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	//SetFilePointer(tempSourceFile, offsetBytes, NULL, FILE_BEGIN);

	char* buffer = new char[bufferBytes];
	DWORD charsCount;

	//Prepare async read
	OVERLAPPED overlap;
	ZeroMemory(&overlap, sizeof(overlap));
	overlap.Offset = offsetBytes;

	//Read async
	ReadFile(tempSourceFile, buffer, bufferBytes, &charsCount, &overlap);


	//Create "answer" file
	HANDLE answerFile = CreateFile(answerFilePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


	//Wait for reading to complete & put data to "answer" file
	WaitForSingleObject(overlap.hEvent, INFINITE);
	WriteFile(answerFile, buffer, bufferBytes, &charsCount, NULL);


	//Clear
	CloseHandle(tempSourceFile);
	delete[] buffer;
	CloseHandle(answerFile);

	DeleteFile(tempFilePath.c_str());
	RemoveDirectory(tempDirectory.c_str());
}
