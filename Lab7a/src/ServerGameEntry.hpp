#pragma once
#include "TicTackToeGame.hpp"


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
		ServerGameEntry(
			ClientSocketTCP<NetworkMessage>&& firstPlayerConnection, std::string firstPlayerName,
			ClientSocketTCP<NetworkMessage>&& secondPlayerConnection, std::string secondPlayerName)
		{
			_firstPlayerName = firstPlayerName;
			_firstPlayerConnection = std::move(firstPlayerConnection);

			_secondPlayerName = secondPlayerName;
			_secondPlayerConnection = std::move(secondPlayerConnection);

			//First is active
			_gameState = TicTackToeGame(_firstPlayerName, _secondPlayerName);

			//Send MMResult to both players
			_firstPlayerConnection.Send(NetworkMessage(NetworkMessage::Type::ServerMMResult, _gameState.ToString()));

			_secondPlayerConnection.Send(NetworkMessage(NetworkMessage::Type::ServerMMResult, _gameState.ToString()));
		}

		void Update(int timeout)
		{
			//Process clients messages
			NetworkMessage* firstMessage = _firstPlayerConnection.TryReceive(timeout);

			if (nullptr != firstMessage && 
				NetworkMessage::Type::ClientGameAction == firstMessage->GetType())
			{
				std::string move = firstMessage->GetData();

				//update game

				_firstPlayerConnection.Send(NetworkMessage(NetworkMessage::Type::ServerMMResult, _gameState.ToString()));
				_secondPlayerConnection.Send(NetworkMessage(NetworkMessage::Type::ServerMMResult, _gameState.ToString()));

				delete firstMessage;
			}


			NetworkMessage* secondMessage = _firstPlayerConnection.TryReceive(timeout);

			if (nullptr != secondMessage && 
				NetworkMessage::Type::ClientGameAction == secondMessage->GetType())
			{
				std::string move = secondMessage->GetData();

				//update game

				_firstPlayerConnection.Send(NetworkMessage(NetworkMessage::Type::ServerMMResult, _gameState.ToString()));
				_secondPlayerConnection.Send(NetworkMessage(NetworkMessage::Type::ServerMMResult, _gameState.ToString()));

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
		//First makes first mvoe, and uses X
		std::string _firstPlayerName;
		ClientSocketTCP<NetworkMessage> _firstPlayerConnection;

		std::string _secondPlayerName;
		ClientSocketTCP<NetworkMessage> _secondPlayerConnection;

		TicTackToeGame _gameState;
	};
}
