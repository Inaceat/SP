#include "stdafx.h"

#include "ChategClient.hpp"



DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	return static_cast<ChategClient*>(lpParameter)->Work();
}



ChategClient::ChategClient()
{
	_mailslot = CreateMailslot(_mailslotName.c_str(), 0, 0, nullptr);

	_messageProcessorThread = CreateThread(nullptr, 0, ThreadProc, this, CREATE_SUSPENDED, nullptr);
}

ChategClient::~ChategClient()
{
	CloseHandle(_mailslot);

	CloseHandle(_messageProcessorThread);
}


void ChategClient::Start()
{
	ResumeThread(_messageProcessorThread);
}


int ChategClient::Work()
{
	return 0;
}
