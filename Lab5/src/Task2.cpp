#include "stdafx.h"

#include "Tasks.hpp"


void Task2::Do()
{
	//Read 4 bytes, starting from 9th
	int offsetBytes = 8;
	int bufferBytes = 4;


	//Get current directory
	StdString startingDirectory = GetCurrentWindowsDirectory();


	//Paths
	StdString tempDirectory = startingDirectory + L"/data/temp";
	StdString tempFilePath = tempDirectory + L"/tempFile";
	StdString answerFilePath = startingDirectory + L"/data/answer.txt";
	StdString sourceFilePath = startingDirectory + L"/data/datafile";


	//Create temp copy
	CreateDirectory(tempDirectory.c_str(), NULL);
	
	CopyFile(sourceFilePath.c_str(), tempFilePath.c_str(), FALSE);


	//Get data from tmp copy
	HANDLE tempSourceFile = CreateFile(tempFilePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	SetFilePointer(tempSourceFile, offsetBytes, NULL, FILE_BEGIN);

	char* buffer = new char[bufferBytes];
	DWORD charsCount;

	ReadFile(tempSourceFile, buffer, bufferBytes, &charsCount, NULL);


	//Create "answer" file
	HANDLE answerFile = CreateFile(answerFilePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


	//Put data to "answer" file
	WriteFile(answerFile, buffer, bufferBytes, &charsCount, NULL);


	//Clear
	CloseHandle(tempSourceFile);
	delete[] buffer;
	CloseHandle(answerFile);

	DeleteFile(tempFilePath.c_str());
	RemoveDirectory(tempDirectory.c_str());
}
