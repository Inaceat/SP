#include "stdafx.h"

#include "Tasks.hpp"


// Глобальные (т.е. разделяемые всеми потоками) переменные: 
// N - размер массива; K - количество потоков; 
// size - размер "проции" массива для обработки одним потоком 
// Sum - накопительная переменная для суммы элементов массива Array 
unsigned N=0,K=0,size=0; 
unsigned Sum=0; 
unsigned* Array = NULL; 

// Функция, запускаемая в потоках 
DWORD WINAPI SummWorker(LPVOID lpStartNumber) 
{ 
	unsigned st = *((unsigned*)lpStartNumber); 
	unsigned partSum = 0;// частичная сумма массива 

	for(unsigned i = size * st; i < size * (st + 1); i++)
		partSum += Array[i]; 

	Sum = Sum + partSum; 

	return 0; 
}


void Task1::Do()
{
	HANDLE* hThreads = NULL; 
	system("chcp 1251");

	std::cout << "Количество элементов массива?" << std::endl << ":";
	std::cin >> N;

	std::cout << "Количество потоков программы?" << std::endl << ":";
	std::cin >> K;

	if ( (N<=0) || (K<=0) || (K>N) || (K>63) )
	{
		std::cerr << "Ошибка параметров. Завершение." << std::endl;
		return; 
	}
	
	Array = new unsigned [N];
	hThreads = new HANDLE[K]; 
	size = N/K;

	//Для теста заполняем массив последовательными элементами 
	for(unsigned i = 0; i < N; i++) 
		Array[i] = i;
	
	// Поиск и печать суммы элементов массива в K потоков 
	for(unsigned i = 0; i < K; i++)
	{
		hThreads[i] = CreateThread(NULL,0,SummWorker,(LPVOID)&i,0,NULL);
	} 
	
	WaitForMultipleObjects(K, hThreads, TRUE, INFINITE);

	std::cout << "Сумма элементов массива равна " << Sum << std::endl;
}
