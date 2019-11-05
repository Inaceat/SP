#include "stdafx.h"

#include "Tasks.hpp"


void Task2::Do()
{
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags |= CREATE_NEW_CONSOLE;

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	CreateProcess(L"../x64/Debug/Lab7.exe", NULL, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
}
