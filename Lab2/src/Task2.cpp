#include "stdafx.h"

#include "Tasks.hpp"

void Task2::Do()
{
	std::cout << "��������� ��� ���, ��� ���������:" << std::endl;

	std::string fileName;
	std::cin >> fileName;


	ShellExecuteA(NULL, "open", ("\"" + fileName + "\"").c_str(), NULL, NULL, SW_SHOWNORMAL);
}
