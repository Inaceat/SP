#pragma once


namespace TTT
{
	enum class Tile
	{
		Empty,
		X,
		O
	};

	class TicTackToeGame
	{
	public:
		TicTackToeGame();


		bool SetTile(int row, int column, Tile newTile);

		Tile GetTile(int row, int column) const;


		std::string ToString();

		TicTackToeGame(std::string stringRepresentation);


		std::string ActivePlayerName() const;


		//TODO Seems like it would be better to split to server & client parts, maybe subclasses of some basic "Game" class
		TicTackToeGame(std::string activeName, std::string waitingName);

		//Ok, this is not good
		void SwapPlayers();


	private:
		std::array<Tile, 9> _gameField;

		std::string _activePlayerName;
		std::string _waitingPlayerName;
	};
}
