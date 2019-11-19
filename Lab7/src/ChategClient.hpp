#pragma once

#include "ChategGUI.hpp"
#include "ChategNetwork.hpp"

#include "ChategServer.hpp"


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
		std::string _clientID;
	
		ChategGUI _gui;
		
		ChategNetwork _network;


		ChategServer* _server = nullptr;
	};
}