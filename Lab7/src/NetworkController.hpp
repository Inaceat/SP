#pragma once

#include "NetworkMessage.hpp"

#include "MailslotConnection.hpp"
#include "NamedPipeConnection.hpp"


namespace Chateg
{
	class NetworkController
	{
	public:
		NetworkController();
		
		~NetworkController();


		void Start(const std::string& serverName);

		bool TryConnectToServerAs(std::string userName, int searchTimeout);


		NetworkMessage* MessageReceive(int timeout);
		
		void MessageSend(NetworkMessage* chategNetworkMessage);
		
		
		void Disconnect();


	private:
		std::string _connectedAsName;
		std::string _clientID;

		bool _domainMode;

		ClientSideMailslotConnection<NetworkMessage>* _outMailslot;

		ServerSideNamedPipeConnection<NetworkMessage>* _inPipe;
	};
}
