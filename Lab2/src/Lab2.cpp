#include "stdafx.h"

#include "Tasks.hpp"


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	while (true)
	{
		std::cout << "1. Делай 1 :" << std::endl;
		std::cout << "2. Делай 2 :" << std::endl;
		std::cout << "3. Делай 3 :" << std::endl;
		std::cout << "4. Делай 4 :" << std::endl;
		std::cout << "5. Делай 5 :" << std::endl;

		char taskNumber;
		do
		{
			taskNumber = _getch();
		} while (taskNumber != '1' && taskNumber != '2' && taskNumber != '3' && taskNumber != '4' && taskNumber != '5');

		std::cout << std::endl;

		switch (taskNumber)
		{
			case '1':
				Task1::Do();
				break;

			case '2':
				Task2::Do();
				break;

			case '3':
				Task3::Do();
				break;

			case '4':
				Task4::Do();
				break;

			case '5':
				Task5::Do();
				break;

			default:
				return 1;
		}
	}
}
