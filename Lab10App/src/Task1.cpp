#include "stdafx.h"


#include "Tasks.hpp"


void Task1::Do()
{
	//char msg[] = "Hello from explicitly loaded Dll!";

	HMODULE libraryA = LoadLibrary(L"../x64/Debug/Lab10aLib.dll");//Yeah, hard-code, nice...
	if (nullptr == libraryA)
	{
		std::cout << "Failed to load Library A: error " << GetLastError() << std::endl;

		return;
	}

	FARPROC sayProcAddress = GetProcAddress(libraryA, "SayMessage");
	if (nullptr == sayProcAddress)
	{
		std::cout << "Failed to get SayMessage address: error " << GetLastError() << std::endl;

		FreeLibrary(libraryA);

		return;
	}

	FARPROC message = GetProcAddress(libraryA, "Message");
	if (nullptr == sayProcAddress)
	{
		std::cout << "Failed to get Message address: error " << GetLastError() << std::endl;

		FreeLibrary(libraryA);

		return;
	}

	reinterpret_cast<void(*)(char*)>(sayProcAddress)(reinterpret_cast<char*>(message));

	FreeLibrary(libraryA);
}
