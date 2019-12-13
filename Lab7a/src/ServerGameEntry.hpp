#pragma once

#include "TicTackToeGame.hpp"
#include "ClientEntry.hpp"


namespace TTT
{
	class ServerGameEntry
	{
		ServerGameEntry(ServerGameEntry& other) = delete;
		void operator=(ServerGameEntry& other) = delete;

	public:
		ServerGameEntry(ClientEntry&& firstPlayer, ClientEntry&& secondPlayer) :
			_firstPlayer(std::move(firstPlayer)),
			_secondPlayer(std::move(secondPlayer))
		{
			//First is active
			_gameState = TicTackToeGame(_firstPlayer.Name(), _secondPlayer.Name());

			//Send MMResult to both players
			_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerMMResult, _gameState.ToString()));

			_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerMMResult, _gameState.ToString()));
		}

		void Update(int timeout)
		{
			//Process clients messages
			NetworkMessage* firstMessage = _firstPlayer.TryReceive(timeout);

			if (nullptr != firstMessage && 
				NetworkMessage::Type::ClientGameAction == firstMessage->GetType())
			{
				int move = stoi(firstMessage->GetData());

				//Update game
				//If turn is valid
				if (_gameState.SetTile((move - 1) / 3, (move-1)%3, Tile::X))
				{
					//If finished, send game results
					if (_gameState.IsFinished())
					{
						switch (_gameState.Result())
						{
							case Result::Draw:
							{
								_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult, "Draw!"));
								_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult, "Draw!"));

							}break;

							case Result::XWin:
							{
								_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult, "You win!"));
								_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult, "You lose!"));

							}break;

							case Result::OWin:
							{
								_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult, "You lose!"));
								_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult, "You win!"));

							}break;
						}
					}
					else//If not yet finished, swap sides, send turn results
					{
						_gameState.SwapPlayers();

						_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameState, _gameState.ToString()));
						_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameState, _gameState.ToString()));
					}
				}
				else//If turn is invalid, send to player made it same state. For second player nothing changes
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

				//Update game
				//If turn is valid
				if (_gameState.SetTile((move - 1) / 3, (move-1)%3, Tile::O))
				{
					//If finished, send game results
					if (_gameState.IsFinished())
					{
						//Send finished game to show
						_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameState, _gameState.ToString()));
						_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameState, _gameState.ToString()));

						switch (_gameState.Result())
						{
							case Result::Draw:
							{
								_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult, "Draw!"));
								_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult, "Draw!"));

							}break;

							case Result::XWin:
							{
								_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult, "You win!"));
								_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult, "You lose!"));

							}break;

							case Result::OWin:
							{
								_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult, "You lose!"));
								_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameResult, "You win!"));

							}break;
						}
					}
					else//If not yet finished, swap sides, send turn results
					{
						_gameState.SwapPlayers();

						_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameState, _gameState.ToString()));
						_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameState, _gameState.ToString()));
					}
				}
				else//If turn is invalid, send to player made it same state. For second player nothing changes
				{
					_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerGameState, _gameState.ToString()));
				}

				delete secondMessage;
			}

		}

		bool IsFinished() const
		{
			return _gameState.IsFinished();
		}


	private:
		//First makes first move, and uses X
		ClientEntry _firstPlayer;
		ClientEntry _secondPlayer;

		TicTackToeGame _gameState;
	};
}
