#include "stdafx.h"

#include "Tasks.hpp"

void Task1::Do()
{
	std::cout << "��������� ��� � ��������:" << std::endl;

	std::string fileName;
	std::cin >> fileName;


	const std::string notepadExe = "C:\\Windows\\notepad.exe";

	WinExec((notepadExe + " \"" + fileName + "\"").c_str(), SW_SHOWNORMAL);
}
