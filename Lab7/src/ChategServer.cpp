#include "stdafx.h"


#include "ChategServer.hpp"
#include "ChategNetworkMessage.hpp"


namespace Chateg
{
	ChategServer::ChategServer(std::string serverName)
	{
		_mailslot = new ServerSideMailslotConnection<ChategNetworkMessage>("\\\\.\\mailslot\\" + serverName);
	
		_thread = CreateThread(nullptr, 0, MessageProcessingThread, this, CREATE_SUSPENDED, nullptr);
	}
	
	ChategServer::~ChategServer()
	{
		TerminateThread(_thread, 1);//TODO send msg to thread
		CloseHandle(_thread);
	
		for (auto client : _clients)
			delete client;
	
		delete _mailslot;
	}


	void ChategServer::Start()
	{
		ResumeThread(_thread);
	}

	void ChategServer::Stop()
	{

	}


	void ChategServer::ProcessMessages()
	{
		while (true)
		{
			if (!_mailslot->HasMessages())
				continue;
	
			ChategNetworkMessage* message = _mailslot->MessageReceive();
	
			//TODO
			if (ChategNetworkMessage::MessageType::Service == message->Type())//TODO message.IsRegistration() ?
			{
				std::string newClientPipeName = "\\\\.\\pipe\\" + message->Data();
	
				//TODO check for duplicates
				_clients.push_back(new ClientSideNamedPipeConnection<ChategNetworkMessage>(newClientPipeName));
			}
	
			if (ChategNetworkMessage::MessageType::Text == message->Type())//message.IsText()
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
}