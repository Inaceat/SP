#include "stdafx.h"


#include "NetworkController.hpp"


namespace TTT
{
	NetworkController::NetworkController()
	{
		//TODO mb move somewhere else
		WSADATA startupData;
		WSAStartup(MAKEWORD(2, 2), &startupData);
	}

	NetworkController::~NetworkController()
	{
		//TODO mb move somewhere else
		WSACleanup();
	}

	void NetworkController::Start()
	{
	}

	bool NetworkController::TryFindServerAs(std::string userName, int timeout)
	{
		ServerSocketTCP<NetworkMessage> connectionCreator("127.0.0.1:42042");

		BroadcastSenderSocketUDP<NetworkMessage> registrationSender("127.0.0.255:42042"); //TODO do smth with it


		NetworkMessage registrationMessage(NetworkMessage::Type::ClientConnectionAsk, userName);

		registrationSender.Send(registrationMessage);


		ClientSocketTCP<NetworkMessage> connection = connectionCreator.TryAcceptIncomingConnection(timeout);

		if (nullptr == connection)
			return false;

		_serverConnectionSocket = std::move(connection);

		return true;
	}

	void NetworkController::Send(NetworkMessage message)
	{
		_serverConnectionSocket.Send(message);
	}

	NetworkMessage* NetworkController::TryReceive(int timeout)
	{
		return _serverConnectionSocket.TryReceive(timeout);
	}
}
