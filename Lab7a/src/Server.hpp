#pragma once

#include "SocketsTCP.hpp"
#include "SocketsUDP.hpp"
#include "NetworkMessage.hpp"
#include "ServerGameEntry.hpp"


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
				NetworkMessage* message = _registrationReceiver.TryReceive(receiveTimeout, senderAddress);

				if (nullptr != message)
				{
					ClientSocketTCP<NetworkMessage> newClientConnection(senderAddress);

					//If noone waits for game, make client waiting
					if (nullptr == _queuedClientConection)
					{
						_queuedClientConection = std::move(newClientConnection);
						_queuedClientName = message->GetData();
					}
					else
					{
						ServerGameEntry* newGameEntry = new ServerGameEntry(
							(std::move(_queuedClientConection)), _queuedClientName,
							(std::move(newClientConnection)), message->GetData());

						_activeGames.push_back(newGameEntry);
					}

					delete message;
				}

				//Process clients
				for (auto game : _activeGames)
					game->Update(updateTimeout);

				//Remove finished games
				_activeGames.remove_if([](ServerGameEntry* game){ return game->IsFinished(); });
			}
		}

	private:
		bool _isWorking;
		std::thread _workerThread;

		BroadcastReceiverSocketUDP<NetworkMessage> _registrationReceiver;

		ClientSocketTCP<NetworkMessage> _queuedClientConection;
		std::string _queuedClientName;

		std::list<ServerGameEntry*> _activeGames;
	};
}
