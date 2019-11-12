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

	for (auto client : _clients)
		delete client;

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

		//TODO
		if (true)//message.IsRegistration()
		{
			std::string newClientName = "\\\\.\\pipe\\_xXx_MeGa_BoSs_xXx_";

			//TODO check for duplicates
			_clients.push_back(new ClientSideNamedPipeConnection<ChategMessage>(newClientName));
		}

		if (true)//message.IsText()
		{
			for (auto client : _clients)
			{
				client->MessageSend(message);
			}
		}

		delete message;
	}
}


DWORD WINAPI ChategServer::MessageProcessingThread(LPVOID threadParam)
{
	static_cast<ChategServer*>(threadParam)->ProcessMessages();

	return 0;
}