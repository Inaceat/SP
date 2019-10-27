#pragma once


class Task1
{
public:
	static void Do();
};

class Task2
{
public:
	static void Do();
};

class Task3
{
public:
	static void Do();
};


#ifndef UNICODE
	typedef std::string StdString;
	#define OUTPUT std::cout
#else
	typedef std::wstring StdString;
	#define OUTPUT std::wcout
#endif

StdString GetCurrentWindowsDirectory();