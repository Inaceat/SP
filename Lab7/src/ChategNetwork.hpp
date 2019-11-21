#pragma once

#include "ChategNetworkMessage.hpp"

#include "MailslotConnection.hpp"
#include "NamedPipeConnection.hpp"


namespace Chateg
{
	class ChategNetwork
	{
	public:
		ChategNetwork();
		
		~ChategNetwork();


		void Start(const std::string& serverName, const std::string& clientName);


		bool TryConnectToServer(int searchTimeout);


		ChategNetworkMessage* MessageReceive(int timeout);
		
		void MessageSend(ChategNetworkMessage* chategNetworkMessage);
		
		
		void Stop();


	private:
		std::string _clientID;
		std::string _clientName;

		bool _domainMode;

		ClientSideMailslotConnection<ChategNetworkMessage>* _outMailslot;

		ServerSideNamedPipeConnection<ChategNetworkMessage>* _inPipe;
	};
}
