#include "stdafx.h"

#include "Tasks.hpp"


DWORD NumbersPrinter(LPVOID param)
{
	for (auto i = 1; i < 50; ++i)
	{
		std::cout << i << std::endl;
		Sleep(500);
	}

	return 0;
}


void Task3::Do()
{
	DWORD threadID;

	HANDLE thread1 = CreateThread(NULL, 0, NumbersPrinter, nullptr, 0, &threadID);

	HANDLE thread2 = CreateThread(NULL, 0, NumbersPrinter, nullptr, 0, &threadID);
}
