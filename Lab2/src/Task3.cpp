#include "stdafx.h"

#include "Tasks.hpp"

void Task3::Do()
{
	std::cout << "��������� ��������� ����:" << std::endl;

	std::string path;
	std::cin >> path;

	ShellExecuteA(NULL, "explore", ("\"" + path + "\"").c_str(), NULL, NULL, SW_SHOWNORMAL);
}
