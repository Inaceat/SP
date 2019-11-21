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
			switch (message->Type())
			{
				case ChategNetworkMessage::MessageType::Text:
				{
					for (auto client : _clients)
						client->MessageSend(message);

				}break;

				case ChategNetworkMessage::MessageType::Register:
				{
					auto delimPosition = message->Data().find('$');

					std::string newClientPipeName = "\\\\" + message->Data().substr(0, delimPosition) + "\\pipe\\" + message->Data().substr(delimPosition+1);

					//TODO check for duplicates
					_clients.push_back(new ClientSideNamedPipeConnection<ChategNetworkMessage>(newClientPipeName));
				
				}break;
				
				
				case ChategNetworkMessage::MessageType::Unregister:
				{
					auto delimPosition = message->Data().find('$');
					
					std::string unregisteringClientPipeName = "\\\\" + message->Data().substr(0, delimPosition) + "\\pipe\\" + message->Data().substr(delimPosition+1);

					
					auto current = _clients.begin();
					for (; current != _clients.end(); ++current)
					{						
						if (unregisteringClientPipeName == (*current)->Name())
							break;
					}

					delete *current;

					_clients.erase(current);

				}break;
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