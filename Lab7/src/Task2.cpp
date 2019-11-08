#include "stdafx.h"

#include "Tasks.hpp"


void Task2::Do()
{
	//Pipe for child to read
	HANDLE childInputRead;
	HANDLE childInputWrite;

	SECURITY_ATTRIBUTES securityAttributesIn;
	securityAttributesIn.nLength = sizeof(SECURITY_ATTRIBUTES);
	securityAttributesIn.lpSecurityDescriptor = NULL;
	securityAttributesIn.bInheritHandle = TRUE;

	CreatePipe(&childInputRead, &childInputWrite, &securityAttributesIn, 0);


	//Pipe for child to write
	HANDLE childOutputRead;
	HANDLE childOutputWrite;

	SECURITY_ATTRIBUTES securityAttributesOut;
	securityAttributesOut.nLength = sizeof(SECURITY_ATTRIBUTES);
	securityAttributesOut.lpSecurityDescriptor = NULL;
	securityAttributesOut.bInheritHandle = TRUE;

	CreatePipe(&childOutputRead, &childOutputWrite, &securityAttributesOut, 0);


	//Buffer
	int bufferSize = 8192;
	TCHAR* buffer = new TCHAR[bufferSize];
	DWORD bytesRead;


	//Starting same app as child
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.hStdError = childOutputWrite;
	si.hStdInput = childInputRead;
	si.hStdOutput = childOutputWrite;
	si.dwFlags |= STARTF_USESTDHANDLES;


	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	CreateProcess("../x64/Debug/Lab7.exe", NULL, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);


	//Command to start file enumerator
	WriteFile(childInputWrite, "1", sizeof(TCHAR), &bytesRead, NULL);
	Sleep(1000);

	ReadFile(childOutputRead, buffer, bufferSize, &bytesRead, NULL);
	buffer[bytesRead] = 0;


	//Ask user
	std::cout << prefix << "¬водить папку дл€ показать файлы:" << std::endl;
	
	std::string directoryToList;

	std::cin >> directoryToList;
	directoryToList.append(" ");//As std input is read by "std::cin << std::string", we need input to be ended with delimiter symbol.

	//Send user input
	WriteFile(childInputWrite, directoryToList.c_str(), directoryToList.size() * sizeof(char), &bytesRead, NULL);
	Sleep(1000);


	//Read answer
	ReadFile(childOutputRead, buffer, bufferSize, &bytesRead, NULL);
	strcpy_s(strstr(buffer, "\r\n\r\n"), 5, "\r\n\r\n\0");//Next app starting prompt is not the thing we need to show.
	
	printf("%s", buffer);


	//Send exit command
	WriteFile(childInputWrite, "0", sizeof(TCHAR), &bytesRead, NULL);


	WaitForSingleObject(pi.hProcess, INFINITE);


	CloseHandle(childInputRead);
	CloseHandle(childInputWrite);
	CloseHandle(childOutputRead);
	CloseHandle(childOutputWrite);

	delete[] buffer;
}
