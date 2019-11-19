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
		_gui.Start();
	
	
		_clientName = _gui.AskClientName();
	
		_network.Start(_serverName, _clientName);


		const int searchTimeout = 1000;

		bool serverFound = _network.TryConnectToServer(searchTimeout);

		if (!serverFound)
		{
			_server = new ChategServer(_serverName);
			
			_server->Start();

			serverFound = _network.TryConnectToServer(searchTimeout);

			if (!serverFound)//TODO maybe smth?
				throw std::string("AAA can't connect to local server");
		}


		const int timeout = 100;

		bool shouldWork = true;

		while (shouldWork)
		{
			ChategGUICommand* command = _gui.CommandReceive(timeout);
			if (nullptr != command)
			{
				switch (command->Type())
				{
					case ChategGUICommand::CommandType::Message:
					{
						_network.MessageSend(new ChategNetworkMessage("[" + _clientName + "]" + command->Data()));
						break;
					}

					case ChategGUICommand::CommandType::Quit:
					{
						shouldWork = false;
						break;
					}
				}

				delete command;
			}


			ChategNetworkMessage* message = _network.MessageReceive(timeout);
			if (nullptr != message)
			{
				switch (message->Type())
				{
					case ChategNetworkMessage::MessageType::Text:
					{
						_gui.ShowMessage(message->Data());
						break;
					}

					default:
						break;
				}

				delete message;
			}
		}

		_network.MessageSend(new ChategNetworkMessage(ChategNetworkMessage::MessageType::Unregister, _clientName));
	}
}
