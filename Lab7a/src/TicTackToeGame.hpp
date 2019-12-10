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
		TicTackToeGame() :
			_gameField()
		{
			for (auto i = 0; i < _gameField.size(); ++i)
				_gameField[i] = Tile::Empty;
		}


		bool SetTile(int row, int column, Tile newTile)
		{
			
			if (row < 0 || 2 < row ||						//If wrong index
				column < 0 || 2 < column ||
				Tile::Empty == newTile ||					//Or trying to place empty tile
				Tile::Empty != _gameField[3*row + column]	//Or trying to place to already occuipied tile
				)
			{
				return false;
			}

			_gameField[3 * row + column] = newTile;
			
			return true;
		}

		Tile GetTile(int row, int column)
		{
			//If wrong index, PANIC
			if (row < 0 || 2 < row ||
				column < 0 || 2 < column
				)
			{
				throw std::invalid_argument("Wrong index of game field tile!");
			}

			return _gameField[3 * row + column];
		}




	private:
		std::array<Tile, 9> _gameField;
	};
}
