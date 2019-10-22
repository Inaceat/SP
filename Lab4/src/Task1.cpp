#include "stdafx.h"

#include "Tasks.hpp"

//Params for SumWorker thread, ptr to it should be cleared in child thread
struct MultithreadArraySumParams
{
	MultithreadArraySumParams(int* array, int startIndex, int sizeOfPart, CRITICAL_SECTION* resultChangeSection, int* resultAccumulator):
		Array(array), StartIndex(startIndex), SizeOfPart(sizeOfPart), ResultChangeSection(resultChangeSection), ResultAccumulator(resultAccumulator)
	{}

	int* Array;
	int StartIndex;
	int SizeOfPart;

	CRITICAL_SECTION* ResultChangeSection;
	int* ResultAccumulator;
};

// �������, ����������� � ������� 
DWORD WINAPI SumWorkerCS(LPVOID params) 
{ 
	auto threadParams = (MultithreadArraySumParams*)params;

	unsigned partSum = 0; 

	for(int i = 0; i < threadParams->SizeOfPart; i++)
		partSum += threadParams->Array[threadParams->StartIndex + i]; 


EnterCriticalSection(threadParams->ResultChangeSection);
	
	*(threadParams->ResultAccumulator) += partSum;

LeaveCriticalSection(threadParams->ResultChangeSection);


	delete threadParams;

	return 0;
}


void Task1::Do()
{
	int arraySize;
	int threadsNumber;

	std::cout << "���������� ��������� �������?" << std::endl << ":";
	std::cin >> arraySize;

	std::cout << "���������� ������� ���������?" << std::endl << ":";
	std::cin >> threadsNumber;

	if ( (arraySize <= 0) || (threadsNumber <= 0) || (threadsNumber > arraySize) || (threadsNumber > 63) )
	{
		std::cerr << "������ ����������. ����������." << std::endl;
		return; 
	}
	

	int* numbersArray = new int[arraySize];

	//��� ����� ��������� ������ ����������������� ����������, [1, arraySize]
	for(int i = 1; i < arraySize + 1; i++) 
		numbersArray[i - 1] = i;


	int threadArrayPartSize = arraySize / threadsNumber;
	int lastArrayPartSize = arraySize % threadsNumber;


	CRITICAL_SECTION resultProtectionCS;
	InitializeCriticalSection(&resultProtectionCS);

	int* result = new int;
	*result = 0;


	HANDLE* threadHandlesArray = new HANDLE[threadsNumber + 1];

	// ����� � ������ ����� ��������� ������� � K ������� 
	for(int i = 0; i < threadsNumber; i++)
	{
		auto newThreadParams = new MultithreadArraySumParams(numbersArray, i * threadArrayPartSize, threadArrayPartSize, &resultProtectionCS, result);

		threadHandlesArray[i] = CreateThread(NULL, 0, SumWorkerCS, (LPVOID)newThreadParams, 0, NULL);
	}

	if (0 != lastArrayPartSize)
	{
		auto lastThreadParams = new MultithreadArraySumParams(numbersArray, threadsNumber * threadArrayPartSize, lastArrayPartSize, &resultProtectionCS, result);

		threadHandlesArray[threadsNumber] = CreateThread(NULL, 0, SumWorkerCS, (LPVOID)lastThreadParams, 0, NULL);
	}
	
	WaitForMultipleObjects(threadsNumber, threadHandlesArray, TRUE, INFINITE);

	std::cout << "����� ��������� ������� ����� " << *result << std::endl;


	//Closing CS
	DeleteCriticalSection(&resultProtectionCS);

	//Closing threads
	for(int i = 0; i < threadsNumber; i++)
		CloseHandle(threadHandlesArray[i]);

	if (0 != lastArrayPartSize)
		CloseHandle(threadHandlesArray[threadsNumber]);

	delete[] threadHandlesArray;

	//Clearing numbers
	delete result;
	delete[] numbersArray;
}
