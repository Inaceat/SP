#include "stdafx.h"

#include "Tasks.hpp"


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	while (true)
	{
		std::cout << "0. Иди... домой :" << std::endl;
		std::cout << "1. Список файлов на выход :" << std::endl;
		std::cout << "2. Давай файлы через пайпу :" << std::endl;

		char taskNumber;
		do
		{
			taskNumber = _getch();
		} while (taskNumber != '0' && taskNumber != '1' && taskNumber != '2');

		std::cout << std::endl;

		switch (taskNumber)
		{
			case '0':
				return 0;

			case '1':
				Task1::Do();
				break;

			case '2':
				Task2::Do();
				break;

			default:
				return 1;
		}
	}
}
