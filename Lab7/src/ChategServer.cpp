#include "stdafx.h"


#include "ChategServer.hpp"
#include "ChategMessage.hpp"


ChategServer::ChategServer(std::string mailslotName)
{
	_mailslot = new ServerSideMailslotConnection<ChategMessage>(mailslotName);

	_thread = CreateThread(nullptr, 0, MessageProcessingThread, this, CREATE_SUSPENDED, nullptr);
}

ChategServer::~ChategServer()
{
	TerminateThread(_thread, 1);
	CloseHandle(_thread);

	delete _mailslot;
}

void ChategServer::Start()
{
	ResumeThread(_thread);
}


void ChategServer::ProcessMessages()
{
	while (true)
	{
		if (!_mailslot->HasMessages())
			continue;

		ChategMessage* message = _mailslot->MessageReceive();

		std::cout << message->ToString();

		delete message;
	}
}


DWORD WINAPI ChategServer::MessageProcessingThread(LPVOID threadParam)
{
	static_cast<ChategServer*>(threadParam)->ProcessMessages();

	return 0;
}