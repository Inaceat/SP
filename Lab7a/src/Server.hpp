#pragma once

#include "SocketsTCP.hpp"
#include "SocketsUDP.hpp"
#include "NetworkMessage.hpp"
#include "ServerGameEntry.hpp"
#include "ClientEntry.hpp"


namespace TTT
{
	class Server
	{
	public:
		Server() : _isWorking(false),
		           _registrationReceiver("42042")
		{}

		~Server()
		{
			_isWorking = false;
			_workerThread.join();
		}

		void Start()
		{
			_isWorking = true;
			_workerThread = std::thread([this]() { this->ProcessMessages(); });
		}

		void ProcessMessages()
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
					ClientSocketTCP<NetworkMessage> newClientConnection(senderAddress);

					//Add to registered
					_registeredClients.push_back(std::move(ClientEntry(registerMessage->GetData(), std::move(newClientConnection))));

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
								else//Create game
								{
									ServerGameEntry* newGameEntry = new ServerGameEntry(std::move(_queuedClient), std::move(*client));

									_activeGames.push_back(newGameEntry);
								}

								client = _registeredClients.erase(client);

							}break;

							case NetworkMessage::Type::ClientDisconnect:
							{
								client = _registeredClients.erase(client);

							}break;

							default:
								++client;//Not in 'for' because of 'erase'
								break;
						}
					}
					else
						++client;
				}

				//Process games
				for (auto game : _activeGames)
				{
					game->Update(updateTimeout);

					//Move connections from games to registered clients
					if (game->IsFinished())
					{
						_registeredClients.push_back(std::move(game->FirstPlayer()));
						_registeredClients.push_back(std::move(game->SecondPlayer()));
					}
				}
				
				//Remove finished games
				_activeGames.remove_if([](ServerGameEntry* game){ return game->IsFinished(); });
			}
		}

	private:
		bool _isWorking;
		std::thread _workerThread;

		BroadcastReceiverSocketUDP<NetworkMessage> _registrationReceiver;

		std::vector<ClientEntry> _registeredClients;

		ClientEntry _queuedClient;

		std::list<ServerGameEntry*> _activeGames;
	};
}
