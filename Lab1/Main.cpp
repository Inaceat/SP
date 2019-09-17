#include <Windows.h>

#include <tchar.h>
#include <iostream>

int main()
{
	//1
	//2
	wchar_t unicodeString[] = L"Hello World!";
	std::wcout << "L\"" << unicodeString << "\"" << std::endl << std::endl;

	//3
	int bufferSize = WideCharToMultiByte(CP_ACP, NULL, unicodeString, -1, NULL, 0, NULL, NULL);;

	LPSTR result = new CHAR[bufferSize];

	WideCharToMultiByte(CP_ACP, NULL, unicodeString, -1, result, bufferSize, NULL, NULL);

	std::cout << "W/out \"wcout\": " << result << std::endl << std::endl;

	delete[] result;


	//4
	TCHAR someString[] = TEXT("Some String\n");

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	if(NULL != consoleHandle && INVALID_HANDLE_VALUE != consoleHandle)
	{
		DWORD written = 0;

		DWORD stringLength = DWORD(_tcslen(someString));

		WriteConsole(consoleHandle, someString, stringLength, &written, NULL);
	}
		

	system("pause");
	return 0;
}