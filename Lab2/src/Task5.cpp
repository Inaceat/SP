#include "stdafx.h"

#include "Tasks.hpp"

void Task5::Do()
{
	std::cout << "�������� ��������� ������:" << std::endl;

	int pID;
	std::cin >> pID;

	std::cout << "������� ��������� ������ " << pID << " �����:" << std::endl;

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
