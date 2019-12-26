#pragma once


#include "NetworkController.hpp"
#include "Server.hpp"
#include "GUIController.hpp"
#include "TicTackToeGame.hpp"


namespace TTT
{
	class Client
	{
		enum class State
		{
			None,
			InMenu,
			SearchingGame,
			MakingTurn,
			WaitingTurn
		};

	public:
		Client();

		~Client();


		void Start();


	private:
		bool StartServerService(std::string serviceName);


	private:
		NetworkController _netController;
		GUIController _guiController;

		Server* _localServer;

		std::string _userName;

		State _clientState;

		TicTackToeGame _gameState;
	};
}
