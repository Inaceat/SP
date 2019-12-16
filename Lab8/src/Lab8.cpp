#include "stdafx.h"


DWORD FilterIntZeroDivision(DWORD code, EXCEPTION_POINTERS* x)
{
	if(EXCEPTION_INT_DIVIDE_BY_ZERO == code)
		return EXCEPTION_EXECUTE_HANDLER;

	return EXCEPTION_CONTINUE_SEARCH;
}


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int a;
	int b;
	int result;

	while (true)
	{
		std::cout << "Велкам ту кэлькулятор. Разделите числа:" << std::endl;
		std::cout << "Делимайе: ";
		std::cin >> a;


		while (true)
		{
			std::cout << "Делитель: ";
			std::cin >> b;

			__try
			{
				result = a / b;

				break;
			}
			__except(FilterIntZeroDivision(GetExceptionCode(), GetExceptionInformation()))
			{
				std::cout << "Ну вот зачем ты на ноль делишь? Чего ты добился? Доволен, да?" << std::endl;
				std::cout << "Давай по новой, чо..." << std::endl;
			}
		}
	

		std::cout << "Маладэц, харашо раздэлил!" << std::endl;
		std::cout << "Палучилась " << result << std::endl;
	}

    return 0;
}