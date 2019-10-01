#include "stdafx.h"

#include "Tasks.hpp"


DWORD ProcessorWarmer(LPVOID param)
{
	while (true)
	{

	}
}


void Task1::Do()
{
	DWORD threadID;
	
	HANDLE newThread = CreateThread(NULL, 0, ProcessorWarmer, nullptr, 0, &threadID);

	std::cout << "Ничегонеделатель №" << threadID << " создан" << std::endl;
}
