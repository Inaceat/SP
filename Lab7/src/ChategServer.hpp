#pragma once

#include "NetworkMessage.hpp"


#include "MailslotConnection.hpp"
#include "NamedPipeConnection.hpp"


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
		static DWORD WINAPI MessageProcessingThread(LPVOID threadParam);
		
		void ProcessMessages();
	
	private:
		HANDLE _thread;

		ServerSideMailslotConnection<NetworkMessage>* _mailslot;
	
		std::vector<ClientSideNamedPipeConnection<NetworkMessage>*> _clients;
	};
}
