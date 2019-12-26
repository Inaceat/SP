#pragma once


#include "SocketsUDP.hpp"
#include "NetworkMessage.hpp"
#include "ServerGameEntry.hpp"
#include "ClientEntry.hpp"


namespace TTT
{
	class Server
	{
	public:
		Server();

		~Server();


		void Start();


		void ProcessMessages();


	private:
		bool _isWorking;
		std::thread _workerThread;

		BroadcastReceiverSocketUDP<NetworkMessage> _registrationReceiver;

		std::vector<ClientEntry> _registeredClients;

		ClientEntry _queuedClient;

		std::list<ServerGameEntry*> _activeGames;
	};
}
