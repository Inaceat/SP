#include "stdafx.h"




int main()
{
	char msg[] = "hello";

	HMODULE libraryA = LoadLibrary(L"../x64/Debug/Lab10aLib.dll");//Yeah, hard-code, nice...
	if (nullptr == libraryA)
	{
		std::cout << "Failed to load Library A: error " << GetLastError() << std::endl;

		return -1;
	}

	FARPROC sayProcAddress = GetProcAddress(libraryA, "SayMessage");
	if (nullptr == sayProcAddress)
	{
		std::cout << "Failed to get SayMessage address: error " << GetLastError() << std::endl;

		FreeLibrary(libraryA);

		return -1;
	}



	reinterpret_cast<void(*)(char*)>(sayProcAddress)(msg);

	FreeLibrary(libraryA);

    return 0;
}
