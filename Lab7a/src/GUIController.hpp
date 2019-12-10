#pragma once


#pragma once
#include "UserCommand.hpp"
#include "TicTackToeGame.hpp"


namespace TTT
{
	class GUIController
	{
	public:
		GUIController()
		{

		}


		void Start()
		{

		}


		std::string AskUserName()
		{
			return "DummyName";
		}

		UserCommand* TryGetUserCommand(int timeout)
		{
			return nullptr;
		}


		void ShowNetworkStatus(std::string newStatus)
		{
			std::cout << "Net: " << newStatus << std::endl;
		}

		void ShowMenu()
		{
			std::cout << "0. Exit." << std::endl << "1. Find game." << std::endl;
		}

		void ShowGameField(TicTackToeGame gameState)
		{
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

	private:

	};
}
