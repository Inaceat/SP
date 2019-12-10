#pragma once


#pragma once


#include "SocketsTCP.hpp"
#include "SocketsUDP.hpp"


#include "NetworkMessage.hpp"


namespace TTT
{
	class NetworkController
	{
	public:
		NetworkController()
		{
			//TODO mb move somewhere else
			WSADATA startupData;
			WSAStartup(MAKEWORD(2, 2), &startupData);
		}

		~NetworkController()
		{
			//TODO mb move somewhere else
			WSACleanup();
		}


		void Start()
		{

		}


		bool TryFindServer(int timeout)
		{
			ServerSocketTCP<NetworkMessage> connectionCreator("127.0.0.1:42042");

			BroadcastSenderSocketUDP<NetworkMessage> registrationSender("127.0.0.255:42042");//TODO do smth with it


			NetworkMessage registrationMessage(NetworkMessage::Type::ClientConnectionAsk, "Hello");

			registrationSender.Send(registrationMessage);


			auto connection = connectionCreator.TryAcceptIncomingConnection(timeout);

			if (nullptr == connection)
				return false;

			_connection = std::move(connection);

			return true;
		}

		void Send(NetworkMessage message)
		{
			
		}

		NetworkMessage* TryReceive(int timeout)
		{
			return nullptr;
		}


	private:
		ClientSocketTCP<NetworkMessage> _connection;
	};
}
