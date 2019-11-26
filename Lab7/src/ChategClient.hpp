#pragma once


#include "NetworkController.hpp"

#include "ChategServer.hpp"
#include "GUIController.hpp"


namespace Chateg
{
	class ChategClient
	{
	public:
		ChategClient();
	
		~ChategClient();
	
	
		void Start();
	
	
	private:
		std::string _serverName;
		std::string _clientName;
		
		GUIController _guiController;
		NetworkController _network;

		ChategServer* _server = nullptr;
	};
}
