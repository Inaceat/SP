#include "stdafx.h"


#include "SocketsUDP.hpp"
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
		WSACleanup();
	}


	void NetworkController::Start()
	{
	}


	bool NetworkController::TryFindServerAs(std::string userName, int timeout)
	{
		//Finding adapter & address
		char adapterNameSubstr[] =  "thernet";


		//Size of adapters list
		unsigned long listSize = 0;
		GetAdaptersAddresses(AF_INET, 0, nullptr, nullptr, &listSize);

		//Getting adapters
		PIP_ADAPTER_ADDRESSES list = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(new char[listSize]);
		GetAdaptersAddresses(AF_INET, 0, nullptr, list, &listSize);

		//Buffer for strings
		const int bufSize = 1024;//Should be enough for everyone
		char buffer[bufSize];
		
		auto currentAddress = list;

		while (nullptr != currentAddress)
		{
			//Get current adapter address
			sockaddr addressInfo = *(currentAddress->FirstUnicastAddress->Address.lpSockaddr);

			//Convert readable adapter name
			WideCharToMultiByte(CP_ACP, 0, currentAddress->FriendlyName, -1, buffer, bufSize, nullptr, nullptr);
			
			//If name contains certain substring
			if (nullptr != strstr(buffer, adapterNameSubstr))
			{
				//Get c-string representation of IP address
				getnameinfo(&addressInfo, sizeof(sockaddr), buffer, bufSize, nullptr, 0, NI_NUMERICHOST);

				//Stop searching
				break;
			}

			currentAddress = currentAddress->Next;
		}

		delete[] list;

		std::string localIP(buffer);

		std::string localAddress = localIP.append(":").append("42042");
		std::string targetAddress = localIP.substr(0, localIP.find_last_of(".") + 1).append("255").append(":").append("42042");
		
		
		//Connecting
		ServerSocketTCP<NetworkMessage> connectionCreator(localAddress);

		BroadcastSenderSocketUDP<NetworkMessage> registrationSender(localAddress, targetAddress);


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
