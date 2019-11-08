#include "stdafx.h"

#include "Tasks.hpp"


void Task1::Do()
{
	std::string directoryToList;

	std::cout << prefix << "Вводить папку для показать файлы:" << std::endl;
	
	std::cin >> directoryToList;
	directoryToList.append("/*");


	WIN32_FIND_DATA searchData;
	HANDLE searchHandle = FindFirstFile(directoryToList.c_str(), &searchData);

	if (INVALID_HANDLE_VALUE != searchHandle)
	{
		std::stringstream fileList;

		do
		{
			if (false == (searchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				std::string fileName(searchData.cFileName);

				fileList << "  " << prefix << fileName << std::endl;
			}
		}
		while (FindNextFile(searchHandle, &searchData));

		std::cout << fileList.str() << std::endl;

		FindClose(searchHandle);
	}
	else
	{
		std::cout << "  " << prefix << "Неудача... \"" << directoryToList << "\" нинайдена." << std::endl << std::endl;
	}
}
