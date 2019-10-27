#include "stdafx.h"

#include "Tasks.hpp"


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	while (true)
	{
		std::cout << "1. Делай 1 :" << std::endl;

		char taskNumber;
		do
		{
			taskNumber = _getch();
		} while (taskNumber != '1');

		std::cout << std::endl;

		switch (taskNumber)
		{
			case '1':
				Task1::Do();
				break;

			default:
				return 1;
		}
	}
}
