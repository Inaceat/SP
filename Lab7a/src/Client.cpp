#include "stdafx.h"



#include "UserCommand.hpp"

#include "Client.hpp"


namespace TTT
{
	Client::Client():
		_localServer(nullptr),
		_clientState(State::None)
	{
	}

	Client::~Client()
	{
		if (nullptr != _localServer)
			delete _localServer;
	}


	void Client::Start()
	{
		_guiController.Start();
		_netController.Start();

		//Ask user name
		_userName = _guiController.AskUserName();

		//Try find server
		const int serverFindTimeout = 2000;

		_guiController.ShowNetworkStatus("Connecting to server...");

		bool serverFound = _netController.TryFindServerAs(_userName, serverFindTimeout);
		if (!serverFound)
		{
			bool serverStarted = StartServerService("TTTServer");

			if (!serverStarted)
			{
				_guiController.ShowNetworkStatus("Failed to start local server");

				throw std::exception("Can't start server");
			}
			else
			{
				_guiController.ShowNetworkStatus("Connecting to local server...");

				serverFound = _netController.TryFindServerAs(_userName, serverFindTimeout);
				if (!serverFound)
				{
					_guiController.ShowNetworkStatus("Failed to connect to local server");

					throw std::exception("Can't start server");
				}
				else
				{
					_guiController.ShowNetworkStatus("Connected to local");
				}
			}
		}
		else
		{
			_guiController.ShowNetworkStatus("Connected");
		}

		/*if (!serverFound)
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
			else //If failed to connect to local, PANIC
			{
				_guiController.ShowNetworkStatus("Whoops, failed to connect to local server");

				throw std::exception("Can't connect to local");
			}
		}
		else
		{
			_guiController.ShowNetworkStatus("Connected");
		}*/


		//Now we are connected to server, so show menu and start Main Cycle
		_guiController.ShowMenu();
		_clientState = State::InMenu;

		const int mainTimeout = 100;

		bool shouldWork = true;
		while (shouldWork)
		{
			//Try get user command from gui
			UserCommand* command = _guiController.TryGetUserCommand(mainTimeout);
			if (nullptr != command)
			{
				switch (command->GetType())
				{
				case UserCommand::Type::FindGame:
					{
						if (State::InMenu == _clientState)
						{
							_clientState = State::SearchingGame;

							//Switch to searching
							_guiController.ShowSearchingMessage();

							//Send request to server
							_netController.Send(NetworkMessage(NetworkMessage::Type::ClientMMAsk, ""));
						}
					}
					break;

				case UserCommand::Type::DoGameAction:
					{
						if (State::MakingTurn == _clientState)
						{
							_clientState = State::WaitingTurn;

							//Send request to server
							_netController.Send(
								NetworkMessage(NetworkMessage::Type::ClientGameAction, command->GetData()));
						}
					}
					break;

				case UserCommand::Type::Exit:
					{
						if (State::InMenu == _clientState)
						{
							shouldWork = false;
						}
					}
					break;

				default:
					break;
				}

				delete command;
			}

			//Try get message from network
			NetworkMessage* message = _netController.TryReceive(mainTimeout);
			if (nullptr != message)
			{
				//Process message
				switch (message->GetType())
				{
				case NetworkMessage::Type::ServerMMResult:
					{
						if (State::SearchingGame == _clientState)
						{
							TicTackToeGame receivedGameState(message->GetData());

							if (_userName == receivedGameState.ActivePlayerName())
							{
								_clientState = State::MakingTurn;
								_guiController.ShowGameStatus("Make your turn... wisely");
							}
							else
							{
								_clientState = State::WaitingTurn;
								_guiController.ShowGameStatus("Wait for your turn");
							}

							_gameState = receivedGameState;
							_guiController.ShowGame(_gameState);
						}
					}
					break;

				case NetworkMessage::Type::ServerGameState:
					{
						if (State::WaitingTurn == _clientState)
						{
							TicTackToeGame receivedGameState(message->GetData());

							if (_userName == receivedGameState.ActivePlayerName())
							{
								_clientState = State::MakingTurn;

								_guiController.ShowGameStatus("Make your turn... wisely");
							}
							else
							{
								_guiController.ShowGameStatus("Wait for your turn");
							}

							_gameState = receivedGameState;
							_guiController.ShowGame(_gameState);
						}
					}
					break;

				case NetworkMessage::Type::ServerGameResult:
					{
						if (State::WaitingTurn == _clientState || State::MakingTurn == _clientState)
						{
							_clientState = State::InMenu;

							std::string gameField = message->GetData().substr(0, message->GetData().find("#"));
							std::string gameResult = message->GetData().substr(message->GetData().find("#") + 1,
							                                                   std::string::npos);

							//Show game result & menu
							_guiController.ShowGame(TicTackToeGame(gameField));
							_guiController.ShowGameResult(gameResult);
							_guiController.ShowMenu();
						}
					}
					break;

				default:
					break;
				}

				delete message;
			}
		}

		//Send deregistration message to server
		_netController.Send(NetworkMessage(NetworkMessage::Type::ClientDisconnect, ""));
	}


	bool Client::StartServerService(std::string serviceName)
	{
		char buffer[300];
		GetCurrentDirectoryA(300, buffer);

		_guiController.ShowNetworkStatus("in " + std::string(buffer));

		std::string serviceBinaryPath = "F:\\Projects\\SP\\x64\\Debug\\Lab11.exe";//Yeah, yeah...

		SC_HANDLE scm = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
		if (nullptr == scm)
		{
			_guiController.ShowNetworkStatus("SCM failed to open: " + std::to_string(GetLastError()));
			return false;
		}

		SC_HANDLE serverService = OpenServiceA(scm, serviceName.c_str(), SERVICE_START);
		//If failed to open
		if (nullptr == serverService)
		{
			//If no such service, create one
			if (ERROR_SERVICE_DOES_NOT_EXIST == GetLastError())
			{
				serverService = CreateServiceA(scm, serviceName.c_str(), serviceName.c_str(), 
					SERVICE_START, SERVICE_WIN32_OWN_PROCESS, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
					serviceBinaryPath.c_str(), 
					nullptr, nullptr, nullptr, nullptr, nullptr);

				if (nullptr == serverService)
				{
					_guiController.ShowNetworkStatus("Service failed to create: " + std::to_string(GetLastError()));
					return false;
				}
			}
			else//panic
			{
				_guiController.ShowNetworkStatus("Service failed to open: " + std::to_string(GetLastError()));
				return false;
			}
		}

		//Start service
		//If failed to start not because of being already running
		if(!StartServiceA(serverService, 0, nullptr) && ERROR_SERVICE_ALREADY_RUNNING != GetLastError())
		{
			_guiController.ShowNetworkStatus("Service failed to start: " + std::to_string(GetLastError()));
			return false;		
		}
		
		return true;
	}
}
