#pragma once


#include "UserCommand.hpp"
#include "TicTackToeGame.hpp"
#include "InputController.hpp"


namespace TTT
{
	class GUIController
	{
		enum class GUIElement
		{
			None,
			Menu,
			GameField
		};

	public:
		GUIController();

		~GUIController();


		void Start();


		std::string AskUserName();

		UserCommand* TryGetUserCommand(int timeout);


		void ShowNetworkStatus(std::string newStatus);

		void ShowMenu();

		void ShowSearchingMessage();

		void ShowGame(TicTackToeGame gameState);

		void ShowGameStatus(std::string newStatus);

		void ShowGameResult(std::string resultMessage);


	private:
		InputController* _inputController;

		GUIElement _activeElement;
	};
}
