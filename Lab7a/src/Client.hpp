#pragma once


#include "NetworkController.hpp"
#include "Server.hpp"
#include "GUIController.hpp"
#include "UserCommand.hpp"
#include "TicTackToeGame.hpp"


namespace TTT
{
	class Client
	{
		enum class State
		{
			None,
			InMenu,
			SearchingGame,
			MakingTurn,
			WaitingTurn
		};

	public:
		Client() :
			_localServer(nullptr),
			_clientState(State::None)
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
						}break;

						case UserCommand::Type::DoGameAction:
						{
							if (State::MakingTurn == _clientState)
							{
								_clientState = State::WaitingTurn;

								//Send request to server
								_netController.Send(NetworkMessage(NetworkMessage::Type::ClientGameAction, command->GetData()));
							}
						}break;

						case UserCommand::Type::Exit:
						{
							if (State::InMenu == _clientState)
							{
								shouldWork = false;

								//Send deregistration message to server
								std::cout << "exit" << std::endl;
							}
						}break;

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
						}break;

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
						}break;

						case NetworkMessage::Type::ServerGameResult:
						{
							if (State::WaitingTurn == _clientState || State::MakingTurn == _clientState)
							{
								_clientState = State::InMenu;

								std::string gameField = message->GetData().substr(0, message->GetData().find("#"));
								std::string gameResult = message->GetData().substr(message->GetData().find("#") + 1, std::string::npos);

								//Show game result & menu
								_guiController.ShowGame(TicTackToeGame(gameField));
								_guiController.ShowGameResult(gameResult);
								_guiController.ShowMenu();
							}
						}break;

						default: 
							break;
					}

					delete message;
				}
			}
		}

	private:
		NetworkController _netController;
		GUIController _guiController;

		Server* _localServer;

		std::string _userName;

		State _clientState;

		TicTackToeGame _gameState;
	};
}
