#include "stdafx.h"

#include "Tasks.hpp"


void MyCopyFile(LPCTSTR sourceFileName, LPCTSTR destinationFileName)
{
	HANDLE source = CreateFile(sourceFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE destination = CreateFile(destinationFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


	DWORD fileBytes = GetFileSize(source, NULL);

	TCHAR* fileData = new TCHAR[fileBytes];


	DWORD charsCount;
	ReadFile(source, fileData, fileBytes, &charsCount, NULL);

	WriteFile(destination, fileData, fileBytes, &charsCount, NULL);


	delete[] fileData;
	CloseHandle(source);
	CloseHandle(destination);
}


void Task3::Do()
{
	StdString startingDirectory = GetCurrentWindowsDirectory();


	//Paths
	StdString sourceFilePath = startingDirectory + L"/data/datafile";

	StdString libCopyFilePath = startingDirectory + L"/data/CopyFile.txt";
	StdString myCopyFilePath = startingDirectory + L"/data/MyCopyFile.txt";


	auto start = std::chrono::steady_clock::now();

	CopyFile(sourceFilePath.c_str(), libCopyFilePath.c_str(), TRUE);

	auto end = std::chrono::steady_clock::now();

	std::cout << "CopyFile function copied in   " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns" << std::endl;



	start = std::chrono::steady_clock::now();

	MyCopyFile(sourceFilePath.c_str(), myCopyFilePath.c_str());

	end = std::chrono::steady_clock::now();

	std::cout << "MyCopyFile function copied in " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns" << std::endl;


	
}
