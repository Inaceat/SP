#include "stdafx.h"

#include "Tasks.hpp"

void Task4::Do()
{
	std::cout << "Убивать номер:" << std::endl;

	int pID;
	std::cin >> pID;

	HANDLE victim = OpenProcess(PROCESS_TERMINATE, FALSE, pID);

	TerminateProcess(victim, 0);
}
