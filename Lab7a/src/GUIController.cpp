#include "stdafx.h"


#include "GUIController.hpp"


namespace TTT
{
	GUIController::GUIController():
		_inputController(new InputController()),
		_activeElement(GUIElement::None)
	{
	}

	GUIController::~GUIController()
	{
		delete _inputController;
	}


	void GUIController::Start()
	{
		_inputController->Start();
	}


	std::string GUIController::AskUserName()
	{
		std::cout << "Name: ";

		std::string userName;
		std::cin >> userName;

		return userName;
	}

	UserCommand* GUIController::TryGetUserCommand(int timeout)
	{
		InputMessage* inputMessagePtr = _inputController->TryGetUserInput(timeout);

		if (nullptr == inputMessagePtr)
			return nullptr;

		InputMessage inputMessage = *inputMessagePtr;

		delete inputMessagePtr;


		switch (_activeElement)
		{
		case GUIElement::None:
			{
				return nullptr;
			}

		case GUIElement::Menu:
			{
				if (inputMessage.IsDigitInput() && inputMessage.Digit() == '0')
					return new UserCommand(UserCommand::Type::Exit, "0");

				if (inputMessage.IsDigitInput() && inputMessage.Digit() == '1')
					return new UserCommand(UserCommand::Type::FindGame, "1");

				return nullptr;
			}

		case GUIElement::GameField:
			{
				if (inputMessage.IsDigitInput())
				{
					std::string action(1, inputMessage.Digit());

					return new UserCommand(UserCommand::Type::DoGameAction, action);
				}

				return nullptr;
			}

		default:
			return nullptr;
		}
	}


	void GUIController::ShowNetworkStatus(std::string newStatus)
	{
		std::cout << "Net: " << newStatus << std::endl;
	}

	void GUIController::ShowMenu()
	{
		_activeElement = GUIElement::Menu;

		std::cout << "0. Exit." << std::endl << "1. Find game." << std::endl;
	}

	void GUIController::ShowSearchingMessage()
	{
		_activeElement = GUIElement::None;

		std::cout << "Searching..." << std::endl;
	}

	void GUIController::ShowGame(TicTackToeGame gameState)
	{
		_activeElement = GUIElement::GameField;

		std::cout << "-------" << std::endl;

		for (auto i = 0; i < 3; ++i)
		{
			std::cout << "|";

			for (auto j = 0; j < 3; ++j)
			{
				auto tile = gameState.GetTile(i, j);

				char tileSymbol = ' ';

				switch (tile)
				{
				case Tile::Empty:
					tileSymbol = ' ';
					break;

				case Tile::X:
					tileSymbol = 'X';
					break;

				case Tile::O:
					tileSymbol = 'O';
					break;
				}

				std::cout << tileSymbol << "|";
			}

			std::cout << std::endl << "-------" << std::endl;
		}
	}

	void GUIController::ShowGameStatus(std::string newStatus)
	{
		std::cout << newStatus << std::endl;
	}

	void GUIController::ShowGameResult(std::string resultMessage)
	{
		std::cout << resultMessage << std::endl;
	}
}
