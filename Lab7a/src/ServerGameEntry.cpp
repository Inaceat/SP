#include "stdafx.h"


#include "ServerGameEntry.hpp"


namespace TTT
{
	ServerGameEntry::ServerGameEntry(ClientEntry&& firstPlayer, ClientEntry&& secondPlayer):
		_firstPlayer(std::move(firstPlayer)),
		_secondPlayer(std::move(secondPlayer)),
		_gameResult(Result::None)
	{
		//First is active
		_gameState = TicTackToeGame(_firstPlayer.Name(), _secondPlayer.Name());

		//Send MMResult to both players
		_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerMMResult, _gameState.ToString()));

		_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerMMResult, _gameState.ToString()));
	}


	void ServerGameEntry::Update(int timeout)
	{
		//Process clients messages
		NetworkMessage* firstMessage = _firstPlayer.TryReceive(timeout);

		if (nullptr != firstMessage &&
			NetworkMessage::Type::ClientGameAction == firstMessage->GetType())
		{
			int move = stoi(firstMessage->GetData());

			int moveRow = (move - 1) / 3;
			int moveColumn = (move - 1) % 3;

			//Update game
			//If turn is valid
			if (_gameState.SetTile(moveRow, moveColumn, Tile::X))
			{
				//Update game result
				if (Result::XWin == CheckRow(moveRow) || //Check row
					Result::XWin == CheckColumn(moveColumn) || //Check column
					Result::XWin == CheckDiagonals()) //Check diagonals
				{
					_gameResult = Result::XWin;
				}
				else if (IsFieldFull()) //Check if field is full
				{
					_gameResult = Result::Draw;
				}

				//Check result
				switch (_gameResult)
				{
					//If finished, send game results
				case Result::Draw:
					{
						_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult,
						                                 _gameState.ToString() + "#Draw!"));
						_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult,
						                                  _gameState.ToString() + "#Draw!"));
					}
					break;

				case Result::XWin:
					{
						_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult,
						                                 _gameState.ToString() + "#You win!"));
						_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult,
						                                  _gameState.ToString() + "#You lose!"));
					}
					break;

				case Result::OWin:
					{
						_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult,
						                                 _gameState.ToString() + "#You lose!"));
						_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult,
						                                  _gameState.ToString() + "#You win!"));
					}
					break;

					//If not yet finished, swap sides, send turn results
				case Result::None:
					{
						_gameState.SwapPlayers();

						_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameState, _gameState.ToString()));
						_secondPlayer.
							Send(NetworkMessage(NetworkMessage::Type::ServerGameState, _gameState.ToString()));
					}
					break;
				}
			}
			else //If turn is invalid, send to player made it same state. For second player nothing changes
			{
				_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameState, _gameState.ToString()));
			}

			delete firstMessage;
		}


		NetworkMessage* secondMessage = _secondPlayer.TryReceive(timeout);

		if (nullptr != secondMessage &&
			NetworkMessage::Type::ClientGameAction == secondMessage->GetType())
		{
			int move = stoi(secondMessage->GetData());

			int moveRow = (move - 1) / 3;
			int moveColumn = (move - 1) % 3;

			//Update game
			//If turn is valid
			if (_gameState.SetTile(moveRow, moveColumn, Tile::O))
			{
				//Update game result
				if (Result::OWin == CheckRow(moveRow) || //Check row
					Result::OWin == CheckColumn(moveColumn) || //Check column
					Result::OWin == CheckDiagonals()) //Check diagonals
				{
					_gameResult = Result::OWin;
				}
				else if (IsFieldFull()) //Check if field is full
				{
					_gameResult = Result::Draw;
				}

				//Check result
				switch (_gameResult)
				{
					//If finished, send game results
				case Result::Draw:
					{
						_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult,
						                                 _gameState.ToString() + "#Draw!"));
						_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult,
						                                  _gameState.ToString() + "#Draw!"));
					}
					break;

				case Result::XWin:
					{
						_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult,
						                                 _gameState.ToString() + "#You win!"));
						_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult,
						                                  _gameState.ToString() + "#You lose!"));
					}
					break;

				case Result::OWin:
					{
						_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult,
						                                 _gameState.ToString() + "#You lose!"));
						_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult,
						                                  _gameState.ToString() + "#You win!"));
					}
					break;

					//If not yet finished, swap sides, send turn results
				case Result::None:
					{
						_gameState.SwapPlayers();

						_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameState, _gameState.ToString()));
						_secondPlayer.
							Send(NetworkMessage(NetworkMessage::Type::ServerGameState, _gameState.ToString()));
					}
					break;
				}
			}
			else //If turn is invalid, send to player made it same state. For second player nothing changes
			{
				_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameState, _gameState.ToString()));
			}

			delete secondMessage;
		}
	}


	bool ServerGameEntry::IsFinished() const
	{
		return _gameResult != Result::None;
	}


	ClientEntry& ServerGameEntry::FirstPlayer()
	{
		return _firstPlayer;
	}

	ClientEntry& ServerGameEntry::SecondPlayer()
	{
		return _secondPlayer;
	}


	ServerGameEntry::Result ServerGameEntry::CheckRow(int row) const
	{
		if (Tile::X == _gameState.GetTile(row, 0) &&
			Tile::X == _gameState.GetTile(row, 1) &&
			Tile::X == _gameState.GetTile(row, 2))
		{
			return Result::XWin;
		}

		if (Tile::O == _gameState.GetTile(row, 0) &&
			Tile::O == _gameState.GetTile(row, 1) &&
			Tile::O == _gameState.GetTile(row, 2))
		{
			return Result::OWin;
		}

		return Result::Draw;
	}

	ServerGameEntry::Result ServerGameEntry::CheckColumn(int column) const
	{
		if (Tile::X == _gameState.GetTile(0, column) &&
			Tile::X == _gameState.GetTile(1, column) &&
			Tile::X == _gameState.GetTile(2, column))
		{
			return Result::XWin;
		}

		if (Tile::O == _gameState.GetTile(0, column) &&
			Tile::O == _gameState.GetTile(1, column) &&
			Tile::O == _gameState.GetTile(2, column))
		{
			return Result::OWin;
		}

		return Result::Draw;
	}

	ServerGameEntry::Result ServerGameEntry::CheckDiagonals() const
	{
		//Left to right diag
		if (Tile::X == _gameState.GetTile(0, 0) &&
			Tile::X == _gameState.GetTile(1, 1) &&
			Tile::X == _gameState.GetTile(2, 2))
		{
			return Result::XWin;
		}

		if (Tile::O == _gameState.GetTile(0, 0) &&
			Tile::O == _gameState.GetTile(1, 1) &&
			Tile::O == _gameState.GetTile(2, 2))
		{
			return Result::OWin;
		}

		//Right to left diag
		if (Tile::X == _gameState.GetTile(0, 2) &&
			Tile::X == _gameState.GetTile(1, 1) &&
			Tile::X == _gameState.GetTile(2, 0))
		{
			return Result::XWin;
		}

		if (Tile::O == _gameState.GetTile(0, 2) &&
			Tile::O == _gameState.GetTile(1, 1) &&
			Tile::O == _gameState.GetTile(2, 0))
		{
			return Result::OWin;
		}

		return Result::Draw;
	}

	bool ServerGameEntry::IsFieldFull() const
	{
		int emptyTilesCount = 0;

		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				if (Tile::Empty == _gameState.GetTile(i, j))
				{
					++emptyTilesCount;
				}

		return 0 == emptyTilesCount;
	}
}
