#pragma once


#include "NetworkController.hpp"
#include "Server.hpp"
#include "GUIController.hpp"


namespace TTT
{
	class Client
	{
	public:
		Client() :
			_localServer(nullptr)
		{}

		~Client()
		{
			if (nullptr !=_localServer)
				delete _localServer;
		}


		void Start()
		{
			_guiController.Start();
			_netController.Start();

			//Ask user name
			_userName = _guiController.AskUserName();

			//Try find server
			const int serverFindTimeout = 1000;

			_guiController.ShowNetworkStatus("Connecting to server...");

			bool serverFound = _netController.TryFindServerAs(_userName, serverFindTimeout);

			if (!serverFound)
			{
				_guiController.ShowNetworkStatus("Starting local server...");

				//Create own server
				_localServer = new Server();
				_localServer->Start();

				//Connect to local server
				serverFound = _netController.TryFindServerAs(_userName, serverFindTimeout);

				//If connected to local
				if (serverFound)
				{
					_guiController.ShowNetworkStatus("Local server");
				}
				else//If failed to connect to local, PANIC
				{
					_guiController.ShowNetworkStatus("Whoops, failed to connect to local server");

					throw std::exception("Can't connect to local");
				}
			}
			else
			{
				_guiController.ShowNetworkStatus("Connected");
			}


			//Now we are connected to server, so start Main Cycle
			const int mainTimeout = 100;

			while (true)
			{
				//Try get user command from gui
				UserCommand* command = _guiController.TryGetUserCommand(mainTimeout);
				if (nullptr != command)
				{
					switch (command->GetType())
					{
						case UserCommand::Type::Exit:
							break;


						default:
							break;
					}
				}

				//Try get message from network
				NetworkMessage* message = _netController.TryReceive(mainTimeout);
				if (nullptr != message)
				{
					//Process message
					switch (message->GetType())
					{
						case NetworkMessage::Type::ServerMMResult:
							break;

						case NetworkMessage::Type::ServerGameState:
							break;


						default: 
							break;
					}
				}
			}
		}

	private:
		NetworkController _netController;
		GUIController _guiController;

		Server* _localServer;

		std::string _userName;
	};
}
