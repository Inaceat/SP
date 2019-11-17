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

		while (true)
		{
			ChategGUICommand* command = _gui.CommandReceive(timeout);
			if (nullptr != command)
			{
				switch (command->Type())
				{
					case ChategGUICommand::CommandType::Message:
					{
						_network.MessageSend(new ChategNetworkMessage(command->Data()));
						break;
					}

					case ChategGUICommand::CommandType::Quit:
					{
						//TODO smth with this
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

					case ChategNetworkMessage::MessageType::Service:
					{
						//TODO smth with this
						break;
					}
				}

				delete message;
			}
	
			//Sleep
		}
	}
}


/*ChategClient::ChategClient()
{
	

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

	_netReceivedMessages = new SynchronizedQueue<ChategMessage*>;
	_netReceivedEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	//_guiEnteredText = new SynchronizedQueue<std::string>();
	//_gui = new ChategGUI(_guiEnteredText);
}

ChategClient::~ChategClient()
{
	delete _mailslot;

	if (nullptr != _server)
	{
		//_server->Stop();
		delete _server;
	}

	TerminateThread(_thread, 1);
	CloseHandle(_thread);

	delete _pipe;

	delete _netReceivedMessages;
	CloseHandle(_netReceivedEvent);
	//delete _gui;
	//delete _guiEnteredText;
}




void ChategClient::Start()
{
	//HANDLE guiTextEnteredEvent;
	//HANDLE guiExitEvent;
	//
	//HANDLE networkMessageReceivedEvent;
	//
	//
	//
	//HANDLE events[3] = { guiTextEnteredEvent, guiExitEvent, networkMessageReceivedEvent };

	//_gui->Init();

	int i = 0;
	while (i++ < 5)
	{
		std::string text = "text" + std::to_string(i);// _gui->GetEnteredText();

		ChategMessage* msg = new ChategMessage(text);

		_mailslot->MessageSend(msg);

		delete msg;

		try
		{
			auto received = _netReceivedMessages->Pop();

			std::cout << "[PseudoGUI]" << received->ToString() << std::endl;

			delete received;
		}
		catch (std::exception e)
		{
			if ("empty" == e.what())
			{
			}
			else
				throw;
		}

		Sleep(1000);
	}

	//while (true)
	//{
	//	DWORD waitResult = WaitForMultipleObjects(3, events, FALSE, 100);
	//
	//	if (WAIT_OBJECT_0 == waitResult)
	//	{
	//		std::string text = "fromGUI";// _gui->GetEnteredText();
	//
	//		ChategMessage* msg = new ChategMessage(text);
	//
	//		_mailslot->MessageSend(msg);
	//
	//		delete msg;
	//	}
	//	else if (WAIT_OBJECT_0 + 1 == waitResult)
	//		break;
	//	else if (WAIT_OBJECT_0 + 2 == waitResult)
	//	{
	//		//_gui->PrintMessage();
	//	}
	//}
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

		std::cout << "[Receiver]" << message->ToString() << std::endl;

		_netReceivedMessages->Push(message);
	}
}


DWORD WINAPI ChategClient::MessageProcessingThread(LPVOID threadParam)
{
	static_cast<ChategClient*>(threadParam)->ProcessMessages();

	return 0;
}*/
