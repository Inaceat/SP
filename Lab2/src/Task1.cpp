#include "stdafx.h"

#include "Tasks.hpp"
#include <minwinbase.h>
#include <minwinbase.h>

void Task1::Do()
{
	std::cout << "Открывать это в блокноте:" << std::endl;

	std::string fileName;
	std::cin >> fileName;

	//std::string notepadExe = "C:\\Windows\\notepad.exe ";
	//notepadExe += " \"" + fileName + "\"";

	LPCSTR appName = "C:\\Windows\\notepad.exe";
	LPSTR cLine = const_cast<char*>("C:\\Windows\\notepad.exe E:\\Documents\\Coding\\Projects\\SP\\Lab2\\src\\Task1.cpp");


	STARTUPINFOA si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	CreateProcessA(NULL, cLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

	//WinExec((notepadExe + " \"" + fileName + "\"").c_str(), SW_SHOWNORMAL);
}
