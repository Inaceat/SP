#include "stdafx.h"

#include "Tasks.hpp"


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	while (true)
	{
		std::cout << "0. Иди... домой :" << std::endl
			<< "1. Грать в крестоноли хочу :" << std::endl;

		char taskNumber;
		do
		{
			std::cin >> taskNumber;

		} while (taskNumber != '0' && taskNumber != '1');
		

		switch (taskNumber)
		{
			case '0':
				return 0;

			case '1':
				Task1::Do();
				break;

			default:
				return 1;
		}
	}
}
