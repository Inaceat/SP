#include "stdafx.h"


#include "ChategServer.hpp"
#include "NetworkMessage.hpp"


namespace Chateg
{
	ChategServer::ChategServer(std::string serverName)
	{
		_mailslot = new ServerSideMailslotConnection<NetworkMessage>("\\\\.\\mailslot\\" + serverName);
	}
	
	ChategServer::~ChategServer()
	{
		_isWorking = false;
		_workerThread.join();
	
		for (auto client : _clients)
			delete client.second;
	
		delete _mailslot;
	}


	void ChategServer::Start()
	{
		_isWorking = true;
		_workerThread = std::thread([this]() { this->ProcessMessages(); });
	}

	void ChategServer::Stop()
	{

	}


	void ChategServer::ProcessMessages()
	{
		while (_isWorking)
		{
			if (!_mailslot->HasMessages())
				continue;
	
			NetworkMessage* message = _mailslot->MessageReceive();
	
			switch (message->Type())
			{
				case NetworkMessage::MessageType::Text:
					ProcessTextMessage(message);
					break;

				case NetworkMessage::MessageType::Register:
					ProcessRegistrationMessage(message);
					break;
				
				
				case NetworkMessage::MessageType::Unregister:
					ProcessDeregistrationMessage(message);
					break;
			}
	
			delete message;
		}
	}


	void ChategServer::ProcessTextMessage(NetworkMessage* message)
	{
		for (auto client : _clients)
		{
			std::cout << "Sending \"" << message->Text() << "\" from \"" << message->Sender() << "\" to \"" << client.second->Name() << "\" as \"" << client.first << "\"" << std::endl;
			client.second->MessageSend(message);
		}
	}

	void ChategServer::ProcessRegistrationMessage(NetworkMessage* message)
	{
		if (_clients.end() == _clients.find(message->Sender()))
		{
			std::string newClientPipeName = "\\\\" + message->Text() + "\\pipe\\" + message->Sender();

			//std::string newClientPipeName = "\\\\.\\pipe\\" + message->Sender();TODO

			_clients.insert(std::make_pair(message->Sender(), new ClientSideNamedPipeConnection<NetworkMessage>(newClientPipeName)));

			std::cout << "Registered \"" << newClientPipeName << "\" as \"" << message->Sender() << "\"" << std::endl;
		}
	}

	void ChategServer::ProcessDeregistrationMessage(NetworkMessage* message)
	{
		if (_clients.end() != _clients.find(message->Sender()))
		{
			delete _clients[message->Sender()];

			_clients.erase(message->Sender());
		}
	}
}
