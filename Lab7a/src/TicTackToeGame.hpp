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

		Tile GetTile(int row, int column) const
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


		std::string ToString()
		{
			std::string result;

			for (auto tile : _gameField)
			{
				switch (tile) 
				{ 
					case Tile::Empty:
						result.append(" ");
						break;

					case Tile::X:
						result.append("X");
						break;
					
					case Tile::O:
						result.append("O");
						break;
				}
			}

			result.append("$").append(_activePlayerName).append("$").append(_waitingPlayerName);

			return result;
		}

		TicTackToeGame(std::string stringRepresentation)
		{
			std::string fieldString(stringRepresentation, 0, stringRepresentation.find("$"));
			
			std::string names(stringRepresentation,  stringRepresentation.find("$") + 1, std::string::npos);

			_activePlayerName = std::string(names, 0, names.find("$"));
			_waitingPlayerName = std::string(names, names.find("$") + 1, std::string::npos);


			for (auto i = 0; i < _gameField.size(); ++i)
			{
				switch (fieldString[i])
				{
					case ' ':
						_gameField[i] = Tile::Empty;
						break;

					case 'X':
						_gameField[i] = Tile::X;
						break;

					case 'O':
						_gameField[i] = Tile::O;
						break;

					default:
						throw std::invalid_argument("Game field parsing failed: wrong string");
				}
			}
		}

		std::string ActivePlayerName() const
		{
			return _activePlayerName;
		}

		//TODO mb split to classes
		TicTackToeGame(std::string activeName, std::string waitingName)
		{
			for (auto i = 0; i < _gameField.size(); ++i)
				_gameField[i] = Tile::Empty;

			_activePlayerName = activeName;
			_waitingPlayerName = waitingName;
		}

	private:
		std::array<Tile, 9> _gameField;

		std::string _activePlayerName;
		std::string _waitingPlayerName;
	};
}
