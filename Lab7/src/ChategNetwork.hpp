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


		void Start(const std::string & serverName, const std::string& clientID);


		bool TryConnectToServer(int searchTimeout);


		ChategNetworkMessage* MessageReceive(int timeout);
		
		void MessageSend(ChategNetworkMessage* chategNetworkMessage);


	private:
		std::string _clientID;


		ClientSideMailslotConnection<ChategNetworkMessage>* _outMailslot;

		ServerSideNamedPipeConnection<ChategNetworkMessage>* _inPipe;
	};
}
