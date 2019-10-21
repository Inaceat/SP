#include "stdafx.h"

#include "Tasks.hpp"


// ���������� (�.�. ����������� ����� ��������) ����������: 
// N - ������ �������; K - ���������� �������; 
// size - ������ "������" ������� ��� ��������� ����� ������� 
// Sum - ������������� ���������� ��� ����� ��������� ������� Array 
unsigned N=0,K=0,size=0; 
unsigned Sum=0; 
unsigned* Array = NULL; 

// �������, ����������� � ������� 
DWORD WINAPI SummWorker(LPVOID lpStartNumber) 
{ 
	unsigned st = *((unsigned*)lpStartNumber); 
	unsigned partSum = 0;// ��������� ����� ������� 

	for(unsigned i = size * st; i < size * (st + 1); i++)
		partSum += Array[i]; 

	Sum = Sum + partSum; 

	return 0; 
}


void Task1::Do()
{
	HANDLE* hThreads = NULL; 
	system("chcp 1251");

	std::cout << "���������� ��������� �������?" << std::endl << ":";
	std::cin >> N;

	std::cout << "���������� ������� ���������?" << std::endl << ":";
	std::cin >> K;

	if ( (N<=0) || (K<=0) || (K>N) || (K>63) )
	{
		std::cerr << "������ ����������. ����������." << std::endl;
		return; 
	}
	
	Array = new unsigned [N];
	hThreads = new HANDLE[K]; 
	size = N/K;

	//��� ����� ��������� ������ ����������������� ���������� 
	for(unsigned i = 0; i < N; i++) 
		Array[i] = i;
	
	// ����� � ������ ����� ��������� ������� � K ������� 
	for(unsigned i = 0; i < K; i++)
	{
		hThreads[i] = CreateThread(NULL,0,SummWorker,(LPVOID)&i,0,NULL);
	} 
	
	WaitForMultipleObjects(K, hThreads, TRUE, INFINITE);

	std::cout << "����� ��������� ������� ����� " << Sum << std::endl;
}
