#pragma once


#include "TicTackToeGame.hpp"
#include "ClientEntry.hpp"


namespace TTT
{
	class ServerGameEntry
	{
		enum class Result
		{
			None,
			Draw,
			XWin,
			OWin
		};


		ServerGameEntry(ServerGameEntry& other) = delete;
		void operator=(ServerGameEntry& other) = delete;

	public:
		ServerGameEntry(ClientEntry&& firstPlayer, ClientEntry&& secondPlayer);


		void Update(int timeout);


		bool IsFinished() const;


		ClientEntry& FirstPlayer();

		ClientEntry& SecondPlayer();


	private:
		Result CheckRow(int row) const;

		Result CheckColumn(int column) const;

		Result CheckDiagonals() const;

		bool IsFieldFull() const;


	private:
		//First makes first move, and uses X
		ClientEntry _firstPlayer;
		ClientEntry _secondPlayer;

		Result _gameResult;

		TicTackToeGame _gameState;
	};
}
