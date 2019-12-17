// Lab10aLib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


extern "C" __declspec(dllexport) void SayMessage(char* cStringMessage)
{
	std::cout << cStringMessage << std::endl;
}