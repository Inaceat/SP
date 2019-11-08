#include "stdafx.h"

#include "ChategServer.hpp"



ChategServer::ChategServer()
{
	_mailslotName = "\\\\*\\mailslot\\ChAtEg";

	_mailslot = CreateMailslot(_mailslotName.c_str(), 0, 0, nullptr);
}

ChategServer::~ChategServer()
{
	CloseHandle(_mailslot);
}


void ChategServer::Start()
{
	while (true)
	{
		//ReadFile()
	}
}
