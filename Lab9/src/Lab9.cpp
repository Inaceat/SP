#include "stdafx.h"



int main()
{	
	//Initial data
	std::string dataPath("data/");

	std::string oddPartsFileName("OddParts.jpg");
	std::string evenPartsFileName("EvenParts.jpg");

	std::string resultFileName("result.jpg");

	DWORD partSizeInBytes = 32;

	//Work
	//Open parts files
	HANDLE oddPartsFile = CreateFileA((dataPath + oddPartsFileName).c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == oddPartsFile)
	{
		std::cout << "Failed to read odd parts file: error " << GetLastError() << std::endl;

		return -1;
	}
	
	HANDLE evenPartsFile = CreateFileA((dataPath + evenPartsFileName).c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == evenPartsFile)
	{
		std::cout << "Failed to read even parts file: error " << GetLastError() << std::endl;

		CloseHandle(oddPartsFile);

		return -1;
	}


	//Get parts size. Assuming files are small enough for size to fit DWORD.
	LARGE_INTEGER fileSizeData;

	GetFileSizeEx(oddPartsFile, &fileSizeData);
	const DWORD oddSizeInBytes = static_cast<DWORD>(fileSizeData.QuadPart);

	GetFileSizeEx(evenPartsFile, &fileSizeData);
	const DWORD evenSizeInBytes = static_cast<DWORD>(fileSizeData.QuadPart);


	//Allocate memory for parts
	LPVOID oddBuffer = VirtualAlloc(nullptr, oddSizeInBytes, MEM_COMMIT, PAGE_READWRITE);
	if (nullptr == oddBuffer)
	{
		std::cout << "Failed to allocate odd parts buffer: error " << GetLastError() << std::endl;

		CloseHandle(oddPartsFile);
		CloseHandle(evenPartsFile);

		return -1;
	}
	
	LPVOID evenBuffer = VirtualAlloc(nullptr, evenSizeInBytes, MEM_COMMIT, PAGE_READWRITE);
	if (nullptr == evenBuffer)
	{
		std::cout << "Failed to allocate even parts buffer: error " << GetLastError() << std::endl;

		CloseHandle(oddPartsFile);
		CloseHandle(evenPartsFile);

		VirtualFree(oddBuffer, 0, MEM_RELEASE);

		return -1;
	}


	//Read parts to memory TODO ERRORS CHECK
	DWORD oddBytesRead;
	BOOL oddReadResult = ReadFile(oddPartsFile, oddBuffer, oddSizeInBytes, &oddBytesRead, nullptr);
	if (FALSE == oddReadResult || oddBytesRead != oddSizeInBytes)
	{
		std::cout << "Failed to read odd parts file result: error " << GetLastError() << std::endl;

		CloseHandle(oddPartsFile);
		CloseHandle(evenPartsFile);

		VirtualFree(oddBuffer, 0, MEM_RELEASE);
		VirtualFree(evenBuffer, 0, MEM_RELEASE);

		return -1;
	}

	DWORD evenBytesRead;
	BOOL evenReadResult = ReadFile(evenPartsFile, evenBuffer, evenSizeInBytes, &evenBytesRead, nullptr);
	if (FALSE == evenReadResult || evenBytesRead != evenSizeInBytes)
	{
		std::cout << "Failed to read even parts file result: error " << GetLastError() << std::endl;

		CloseHandle(oddPartsFile);
		CloseHandle(evenPartsFile);

		VirtualFree(oddBuffer, 0, MEM_RELEASE);
		VirtualFree(evenBuffer, 0, MEM_RELEASE);

		return -1;
	}
	

	//Close parts files
	CloseHandle(oddPartsFile);
	CloseHandle(evenPartsFile);


	//Calculate result size
	DWORD resultSizeInBytes = oddSizeInBytes + evenSizeInBytes;


	//Allocate memory for result
	LPVOID resultBuffer = VirtualAlloc(nullptr, resultSizeInBytes, MEM_COMMIT, PAGE_READWRITE);
	if (nullptr == resultBuffer)
	{
		std::cout << "Failed to allocate result memory buffer: error " << GetLastError() << std::endl;

		//Delete parts memory
		VirtualFree(oddBuffer, 0, MEM_RELEASE);
		VirtualFree(evenBuffer, 0, MEM_RELEASE);

		return -1;
	}


	//Aggregate parts to result
	DWORD oddBytesLeft = oddSizeInBytes;
	DWORD evenBytesLeft = evenSizeInBytes;

	DWORD oddBytesToCopy;
	DWORD evenBytesToCopy;

	char* currentOddByte = static_cast<char*>(oddBuffer);
	char* currentEvenByte = static_cast<char*>(evenBuffer);
	char* currentResultByte = static_cast<char*>(resultBuffer);

	while (oddBytesLeft > 0 && evenBytesLeft > 0)
	{
		if (oddBytesLeft > 0)
		{
			oddBytesToCopy = (partSizeInBytes < oddBytesLeft) ? partSizeInBytes : oddBytesLeft;

			CopyMemory(currentResultByte, currentOddByte, oddBytesToCopy);
			currentResultByte += oddBytesToCopy;
			currentOddByte += oddBytesToCopy;

			oddBytesLeft -= oddBytesToCopy;
		}

		if (evenBytesLeft > 0)
		{
			evenBytesToCopy = (partSizeInBytes < evenBytesLeft) ? partSizeInBytes : evenBytesLeft;

			CopyMemory(currentResultByte, currentEvenByte, evenBytesToCopy);
			currentResultByte += evenBytesToCopy;
			currentEvenByte += evenBytesToCopy;

			evenBytesLeft -= evenBytesToCopy;
		}
	}


	//Delete parts memory
	VirtualFree(oddBuffer, 0, MEM_RELEASE);
	VirtualFree(evenBuffer, 0, MEM_RELEASE);


	//Create result file TODO ERRORS CHECK
	HANDLE resultFile = CreateFileA((dataPath + resultFileName).c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if(INVALID_HANDLE_VALUE == resultFile)
	{
		std::cout << "Failed to create result file: error " << GetLastError() << std::endl;

		VirtualFree(resultBuffer, 0, MEM_RELEASE);

		return -1;
	}


	//Write result to file
	DWORD resultBytesWritten;
	BOOL writeResult = WriteFile(resultFile, resultBuffer, resultSizeInBytes, &resultBytesWritten, nullptr);
	if (FALSE == writeResult || resultBytesWritten != resultSizeInBytes)
	{
		std::cout << "Failed to write result." << std::endl;
	}


	//Close result file
	CloseHandle(resultFile);


	//Clear result memory
	VirtualFree(resultBuffer, 0, MEM_RELEASE);


	return 0;
}
