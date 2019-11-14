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

	_gui = new ChategGUI();
}

ChategClient::~ChategClient()
{
	delete _mailslot;

	if (nullptr != _server)
		delete _server;

	TerminateThread(_thread, 1);
	CloseHandle(_thread);

	delete _pipe;

	delete _gui;
}




void ChategClient::Start()
{
	HANDLE guiMessageEnteredEvent;
	HANDLE guiExitEvent;

	HANDLE networkMessageReceivedEvent;

	_gui->Init();


	while (true)
	{
		//auto waitResult = WaitForMultipleObjects(,,, 100);

		//if (WAIT_OBJECT_0 == waitResult)
		{
			//GUI & network events
			///...
		}

		
	}
	
	//ChategMessage* msg = new ChategMessage(userText);
	//
	//_mailslot->MessageSend(msg);
	//
	//delete msg;
	//
	//Sleep(4000);
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
