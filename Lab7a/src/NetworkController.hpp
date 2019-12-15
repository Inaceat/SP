#pragma once


#include "SocketsTCP.hpp"


#include "NetworkMessage.hpp"


namespace TTT
{
	class NetworkController
	{
	public:
		NetworkController();

		~NetworkController();


		void Start();


		bool TryFindServerAs(std::string userName, int timeout);


		void Send(NetworkMessage message);

		NetworkMessage* TryReceive(int timeout);


	private:
		ClientSocketTCP<NetworkMessage> _serverConnectionSocket;
	};
}
