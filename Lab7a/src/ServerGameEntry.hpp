#pragma once
#include "TicTackToeGame.hpp"
#include "ClientEntry.hpp"


namespace TTT
{
	class ServerGameEntry
	{
		enum class Tile
		{
			Empty,
			X,
			O
		};

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
				std::string move = firstMessage->GetData();

				//update game

				_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerMMResult, _gameState.ToString()));
				_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerMMResult, _gameState.ToString()));

				delete firstMessage;
			}


			NetworkMessage* secondMessage = _firstPlayer.TryReceive(timeout);

			if (nullptr != secondMessage && 
				NetworkMessage::Type::ClientGameAction == secondMessage->GetType())
			{
				std::string move = secondMessage->GetData();

				//update game

				_firstPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerMMResult, _gameState.ToString()));
				_secondPlayer.Send(NetworkMessage(NetworkMessage::Type::ServerMMResult, _gameState.ToString()));

				delete secondMessage;
			}

		}

		bool IsFinished() const
		{
			if (TTT::Tile::Empty != _gameState.GetTile(2, 2))
			{
				return true;
			}

			return false;
		}


	private:
		//First makes first move, and uses X
		ClientEntry _firstPlayer;
		ClientEntry _secondPlayer;

		TicTackToeGame _gameState;
	};
}
