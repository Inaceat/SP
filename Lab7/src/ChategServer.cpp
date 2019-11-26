#include "stdafx.h"


#include "ChategServer.hpp"
#include "NetworkMessage.hpp"


namespace Chateg
{
	ChategServer::ChategServer(std::string serverName)
	{
		_mailslot = new ServerSideMailslotConnection<NetworkMessage>("\\\\.\\mailslot\\" + serverName);
	
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
		std::cout << "startingMsg";
		while (true)
		{
			if (!_mailslot->HasMessages())
				continue;
	
			NetworkMessage* message = _mailslot->MessageReceive();
	
			//TODO
			switch (message->Type())
			{
				case NetworkMessage::MessageType::Text:
				{
					for (auto client : _clients)
						client->MessageSend(message);

				}break;

				case NetworkMessage::MessageType::Register:
				{
					auto delimPosition = message->Text().find('$');

					std::string newClientPipeName = "\\\\" + message->Text().substr(0, delimPosition) + "\\pipe\\" + message->Text().substr(delimPosition+1);

					for (auto client : _clients)
					{
						if (newClientPipeName == client->Name())
							break;
					}

					_clients.push_back(new ClientSideNamedPipeConnection<NetworkMessage>(newClientPipeName));
				
				}break;
				
				
				case NetworkMessage::MessageType::Unregister:
				{
					auto delimPosition = message->Text().find('$');
					
					std::string unregisteringClientPipeName = "\\\\" + message->Text().substr(0, delimPosition) + "\\pipe\\" + message->Text().substr(delimPosition+1);

					
					auto current = _clients.begin();
					for (; current != _clients.end(); ++current)
					{						
						if (unregisteringClientPipeName == (*current)->Name())
							break;
					}

					if (_clients.end() != current)
					{
						delete *current;

						_clients.erase(current);
					}					

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