#include "stdafx.h"

#include "Tasks.hpp"


void Task1::Do()
{
	std::wstring directoryToList;

	std::wcout << "Вводить папку для показать файлы:" << std::endl;
	std::wcin >> directoryToList;

	directoryToList.append(L"/*");


	WIN32_FIND_DATA searchData;
	HANDLE searchHandle = FindFirstFile(directoryToList.c_str(), &searchData);

	if (INVALID_HANDLE_VALUE != searchHandle)
	{
		do
		{
			if (false == (searchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				std::wstring fileName(searchData.cFileName);

				std::wcout << L"  " << fileName << std::endl;
			}
		}
		while (FindNextFile(searchHandle, &searchData));

		FindClose(searchHandle);
	}
	else
	{
		std::wcout << "Неудача... \"" << directoryToList << "\" нинайдена." << std::endl;
	}

	std::wcout << std::endl;
}
