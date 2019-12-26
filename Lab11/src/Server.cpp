#include "stdafx.h"


#include "SocketsTCP.hpp"

#include "Server.hpp"
#include "Log.hpp"

namespace TTT
{
	Server::Server(): _isWorking(false),
	                  _registrationReceiver("42042")
	{
	}

	Server::~Server()
	{
		_isWorking = false;
		_workerThread.join();
	}


	void Server::Start()
	{
		_isWorking = true;
		_workerThread = std::thread([this]() { this->ProcessMessages(); });
	}


	void Server::ProcessMessages()
	{
		int receiveTimeout = 100;
		int updateTimeout = 100;

		sockaddr_in senderAddress;

		while (_isWorking)
		{
			//Process registration
			NetworkMessage* registerMessage = _registrationReceiver.TryReceive(receiveTimeout, senderAddress);

			if (nullptr != registerMessage)
			{
				char buf[100];
				inet_ntop(AF_INET, &(senderAddress.sin_addr), buf, 100);
				Log("Reg received from " + std::string(buf) + ":" + std::to_string(ntohs(senderAddress.sin_port)));
				

				ClientSocketTCP<NetworkMessage> newClientConnection(std::string(buf) + ":42042");

				if (nullptr != newClientConnection)
				{
					//Add to registered
					_registeredClients.push_back(
						std::move(ClientEntry(registerMessage->GetData(), std::move(newClientConnection))));
				}
				else
					Log("Client connection failed! Error " + std::to_string(GetLastError()));

				delete registerMessage;
			}

			//Process MM
			for (auto client = _registeredClients.begin(); client < _registeredClients.end();)
			{
				NetworkMessage* clientMessage = client->TryReceive(receiveTimeout);
				if (nullptr != clientMessage)
				{
					switch (clientMessage->GetType())
					{
					case NetworkMessage::Type::ClientMMAsk:
						{
							//If noone waits for game, make client waiting
							if (nullptr == _queuedClient)
							{
								_queuedClient = std::move(*client);
							}
							else //Create game
							{
								ServerGameEntry* newGameEntry = new ServerGameEntry(
									std::move(_queuedClient), std::move(*client));

								_activeGames.push_back(newGameEntry);
							}

							client = _registeredClients.erase(client);
						}
						break;

					case NetworkMessage::Type::ClientDisconnect:
						{
							client = _registeredClients.erase(client);
						}
						break;

					default:
						++client; //Not in 'for' because of 'erase'
						break;
					}
				}
				else
					++client;
			}

			//Process games
			for (auto game = _activeGames.begin(); game != _activeGames.end();)
			{
				(*game)->Update(updateTimeout);


				if ((*game)->IsFinished())
				{
					//Move connections from games to registered clients
					_registeredClients.push_back(std::move((*game)->FirstPlayer()));
					_registeredClients.push_back(std::move((*game)->SecondPlayer()));

					//Delete game entry
					delete *game;

					game = _activeGames.erase(game);
				}
				else
				{
					++game;
				}
			}

			//Remove finished games
			_activeGames.remove_if([](ServerGameEntry* game) { return game->IsFinished(); });
		}
	}
}
