#pragma once

#include "NetworkMessage.hpp"


#include "MailslotConnection.hpp"
#include "NamedPipeConnection.hpp"
#include <map>


namespace Chateg
{
	class ChategServer
	{
	public:
		ChategServer(std::string serverName);
	
		~ChategServer();
	
	
		void Start();
		
		void Stop();


	private:		
		void ProcessMessages();

		void ProcessTextMessage(NetworkMessage* message);
		void ProcessRegistrationMessage(NetworkMessage* message);
		void ProcessDeregistrationMessage(NetworkMessage* message);

	private:
		bool _isWorking;
		std::thread _workerThread;

		ServerSideMailslotConnection<NetworkMessage>* _mailslot;
	
		std::map<std::string, ClientSideNamedPipeConnection<NetworkMessage>*> _clients;
	};
}
