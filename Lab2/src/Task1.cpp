#include "stdafx.h"

#include "Tasks.hpp"
#include <minwinbase.h>

void Task1::Do()
{
	std::cout << "Открывать это в блокноте:" << std::endl;

	std::string fileName;
	std::cin >> fileName;

	fileName = "\"" + fileName + "\"";


	std::string appName = "C:\\Windows\\notepad.exe";


	auto fullCommandLine = appName + " " + fileName;
	
	LPSTR cLine = const_cast<char*>(fullCommandLine.c_str());


	STARTUPINFOA si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	CreateProcessA(NULL, cLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
}
