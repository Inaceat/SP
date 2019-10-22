#include "stdafx.h"

#include "Tasks.hpp"

//Params for SumWorker thread, ptr to it should be cleared in child thread
struct MultithreadArraySumParams
{
	MultithreadArraySumParams(int* array, int startIndex, int sizeOfPart, HANDLE* resultChangeMutex, int* resultAccumulator) :
		Array(array), StartIndex(startIndex), SizeOfPart(sizeOfPart), ResultChangeMutex(resultChangeMutex), ResultAccumulator(resultAccumulator)
	{}

	int* Array;
	int StartIndex;
	int SizeOfPart;

	HANDLE* ResultChangeMutex;
	int* ResultAccumulator;
};

// Функция, запускаемая в потоках 
DWORD WINAPI SumWorkerMutex(LPVOID params)
{
	auto threadParams = (MultithreadArraySumParams*)params;

	unsigned partSum = 0;

	for (int i = 0; i < threadParams->SizeOfPart; i++)
		partSum += threadParams->Array[threadParams->StartIndex + i];


WaitForSingleObject(*(threadParams->ResultChangeMutex), INFINITE);

	*(threadParams->ResultAccumulator) += partSum;

ReleaseMutex(*(threadParams->ResultChangeMutex));
	

	delete threadParams;

	return 0;
}


void Task2::Do()
{
	int arraySize;
	int threadsNumber;

	std::cout << "Количество элементов массива?" << std::endl << ":";
	std::cin >> arraySize;

	std::cout << "Количество потоков программы?" << std::endl << ":";
	std::cin >> threadsNumber;

	if ((arraySize <= 0) || (threadsNumber <= 0) || (threadsNumber > arraySize) || (threadsNumber > 63))
	{
		std::cerr << "Ошибка параметров. Завершение." << std::endl;
		return;
	}


	int* numbersArray = new int[arraySize];

	//Для теста заполняем массив последовательными элементами, [1, arraySize]
	for (int i = 1; i < arraySize + 1; i++)
		numbersArray[i - 1] = i;


	int threadArrayPartSize = arraySize / threadsNumber;
	int lastArrayPartSize = arraySize % threadsNumber;


	HANDLE resultProtectionMutex = CreateMutex(NULL, FALSE, NULL);
	
	int* result = new int;
	*result = 0;


	HANDLE* threadHandlesArray = new HANDLE[threadsNumber + 1];

	// Поиск и печать суммы элементов массива в K потоков 
	for (int i = 0; i < threadsNumber; i++)
	{
		auto newThreadParams = new MultithreadArraySumParams(numbersArray, i * threadArrayPartSize, threadArrayPartSize, &resultProtectionMutex, result);

		threadHandlesArray[i] = CreateThread(NULL, 0, SumWorkerMutex, (LPVOID)newThreadParams, 0, NULL);
	}

	if (0 != lastArrayPartSize)
	{
		auto lastThreadParams = new MultithreadArraySumParams(numbersArray, threadsNumber * threadArrayPartSize, lastArrayPartSize, &resultProtectionMutex, result);

		threadHandlesArray[threadsNumber] = CreateThread(NULL, 0, SumWorkerMutex, (LPVOID)lastThreadParams, 0, NULL);
	}

	WaitForMultipleObjects(threadsNumber, threadHandlesArray, TRUE, INFINITE);

	std::cout << "Сумма элементов массива равна " << *result << std::endl;


	//Closing mutex
	CloseHandle(resultProtectionMutex);

	//Closing threads
	for (int i = 0; i < threadsNumber; i++)
		CloseHandle(threadHandlesArray[i]);

	if (0 != lastArrayPartSize)
		CloseHandle(threadHandlesArray[threadsNumber]);

	delete[] threadHandlesArray;

	//Clearing numbers
	delete result;
	delete[] numbersArray;
}
