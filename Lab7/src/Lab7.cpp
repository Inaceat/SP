#include "stdafx.h"

#include "Tasks.hpp"
#include "GUIRenderer.hpp"
#include "GUIController.hpp"


std::string prefix;

int main()
{
	std::cout << "message" << std::endl;

	auto x = new GUIController();
	Sleep(1000);

	x->Start();
	Sleep(1000);

	x->AskClientName();
	Sleep(1000);



	delete x;



	/*std::random_device rd;
	std::mt19937 gen(rd());
	int id = gen() / 1000000;
	prefix = "[" + std::to_string(id) + "]";

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	while (true)
	{
		std::cout << prefix << "0. Иди... домой :" << std::endl
			<< prefix << "1. Список файлов на выход :" << std::endl
			<< prefix << "2. Давай файлы через пайпу :" << std::endl
			<< prefix << "3. Давай ЧаТеГ :" << std::endl;

		char taskNumber;
		do
		{
			std::cin >> taskNumber;

		} while (taskNumber != '0' && taskNumber != '1' && taskNumber != '2' && taskNumber != '3');

		

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

			case '3':
				Task3::Do();
				break;

			default:
				return 1;
		}
	}*/
}
