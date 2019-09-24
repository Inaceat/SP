#include "stdafx.h"

#include "Tasks.hpp"

void Task1::Do()
{
	std::cout << "Открывать это в блокноте:" << std::endl;

	std::string fileName;
	std::cin >> fileName;

	std::string notepadExe = "C:\\Windows\\notepad.exe ";
	notepadExe += " \"" + fileName + "\"";

	STARTUPINFOA si;
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	CreateProcessA(notepadExe.c_str(), NULL, NULL, NULL, FALSE, DETACHED_PROCESS, NULL, NULL, &si, &pi);

	//WinExec((notepadExe + " \"" + fileName + "\"").c_str(), SW_SHOWNORMAL);
}
