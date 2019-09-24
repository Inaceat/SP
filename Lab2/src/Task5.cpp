#include "stdafx.h"

#include "Tasks.hpp"

void Task5::Do()
{
	std::cout << "Изменять приоритет номера:" << std::endl;

	int pID;
	std::cin >> pID;

	std::cout << "Ставить приоритет номера " << pID << " такой:" << std::endl;

	int neededPriority;
	std::cin >> neededPriority;

	int priorityToSet = NORMAL_PRIORITY_CLASS;

	switch (neededPriority)
	{
		case 0:
			priorityToSet = NORMAL_PRIORITY_CLASS;
			break;

		case 1:
			priorityToSet = ABOVE_NORMAL_PRIORITY_CLASS;
			break;

		case 2:
			priorityToSet = BELOW_NORMAL_PRIORITY_CLASS;
			break;
		
		default:
			std::cout << "Unknown priority!" << std::endl;
	}



	HANDLE victim = OpenProcess(PROCESS_SET_INFORMATION, FALSE, pID);

	SetPriorityClass(victim, priorityToSet);
}
