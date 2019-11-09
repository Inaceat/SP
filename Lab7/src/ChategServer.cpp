#include "stdafx.h"


#include "ChategServer.hpp"


ChategServer::ChategServer(std::string mailslotName)
{
	_mailslotRead = CreateMailslot(mailslotName.c_str(), 0, 0, nullptr);

	_thread = CreateThread(nullptr, 0, MessageProcessingThread, this, CREATE_SUSPENDED, nullptr);
}

ChategServer::~ChategServer()
{
	CloseHandle(_mailslotRead);

	TerminateThread(_thread, 1);
	CloseHandle(_thread);
}

void ChategServer::Start()
{
	ResumeThread(_thread);
}


void ChategServer::ProcessMessages()
{
	DWORD nextSize;

	char* readBuffer = new char[424];
	DWORD readBytes;

	std::string receivedMessage;

	while (true)
	{
		GetMailslotInfo(_mailslotRead, nullptr, &nextSize, nullptr, nullptr);

		if (MAILSLOT_NO_MESSAGE == nextSize)
			continue;

		ReadFile(_mailslotRead, readBuffer, nextSize, &readBytes, nullptr);
		readBuffer[readBytes] = '\0';

		receivedMessage.assign(readBuffer);

		std::cout << receivedMessage << std::endl;

		if ("!q" == receivedMessage)
			break;
	}

	delete[] readBuffer;
}


DWORD WINAPI ChategServer::MessageProcessingThread(LPVOID threadParam)
{
	static_cast<ChategServer*>(threadParam)->ProcessMessages();

	return 0;
}