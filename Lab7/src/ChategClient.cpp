#include "stdafx.h"

#include "ChategClient.hpp"


namespace Chateg
{
	ChategClient::ChategClient()
	{
		_serverName = "ChAtEg";
	}
	
	ChategClient::~ChategClient()
	{
		if (nullptr != _server)
		{
			_server->Stop();

			delete _server;
		}
	}
	
	
	void ChategClient::Start()
	{
		_guiController.Start();
		_guiController.SetNetworkStatus("Offline");
	
		_clientName = _guiController.AskClientName();
	
		_network.Start(_serverName);


		const int searchTimeout = 5000;

		_guiController.SetNetworkStatus("Connecting....");
		bool serverFound = _network.TryConnectToServerAs(_clientName, searchTimeout);

		if (serverFound)
		{
			_guiController.SetNetworkStatus("Connected");
		}
		else
		{
			_server = new ChategServer(_serverName);
			
			_server->Start();

			_guiController.SetNetworkStatus("Trying to connect to local....");
			serverFound = _network.TryConnectToServerAs(_clientName, searchTimeout);

			if (serverFound)
				_guiController.SetNetworkStatus("Local server"); 
			else
			{
				_guiController.SetNetworkStatus("Local server failed to start");
				throw std::exception("Local server failed");
			}
		}


		const int timeout = 100;

		bool shouldWork = true;

		while (shouldWork)
		{
			UserMessage* command = _guiController.TryGetUserCommand(timeout);
			if (nullptr != command)
			{
				//If exit command
				if ("" == command->SenderName())
					break;

				//Else it's text message
				_network.MessageSend(new NetworkMessage(NetworkMessage::MessageType::Text, _clientName, command->Text()));

				delete command;
			}


			NetworkMessage* message = _network.MessageReceive(timeout);
			if (nullptr != message)
			{
				switch (message->Type())
				{
					case NetworkMessage::MessageType::Text:
					{
						_guiController.ShowMessage(new UserMessage(message->Sender(), message->Text()));
						break;
					}

					default:
						break;
				}

				delete message;
			}
		}

		_network.Disconnect();
	}
}
