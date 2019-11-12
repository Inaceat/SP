#include "stdafx.h"

#include "ChategClient.hpp"


ChategClient::ChategClient()
{
	_pipe = new ServerSideNamedPipeConnection<ChategMessage>(_pipeName);

	bool serverFound = TryFindServer();

	if (!serverFound)
	{
		_server = new ChategServer(_mailslotName);
		_server->Start();
	}
	else
	{
		//TODO Registration
	}

	//TODO or move connection elsewhere
	_mailslot = new ClientSideMailslotConnection<ChategMessage>(_mailslotName);

	_thread = CreateThread(nullptr, 0, MessageProcessingThread, this, CREATE_SUSPENDED, nullptr);
	ResumeThread(_thread);
}

ChategClient::~ChategClient()
{
	delete _mailslot;

	if (nullptr != _server)
		delete _server;

	TerminateThread(_thread, 1);
	CloseHandle(_thread);

	delete _pipe;
}


void ChategClient::Start()
{
	std::string userText;

	std::cin >> userText;

	ChategMessage* msg = new ChategMessage(userText);

	_mailslot->MessageSend(msg);

	delete msg;

	Sleep(4000);
}



bool ChategClient::TryFindServer()
{
	return false;
}


void ChategClient::ProcessMessages()
{
	while (true)
	{
		if (!_pipe->HasMessages())
			continue;

		ChategMessage* message = _pipe->MessageReceive();

		std::cout << "[Client]" << message->ToString() << std::endl;

		delete message;
	}
}


DWORD WINAPI ChategClient::MessageProcessingThread(LPVOID threadParam)
{
	static_cast<ChategClient*>(threadParam)->ProcessMessages();

	return 0;
}
