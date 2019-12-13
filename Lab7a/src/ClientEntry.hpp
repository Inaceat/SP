#pragma once


#include "SocketsTCP.hpp"
#include "NetworkMessage.hpp"


namespace TTT
{
	class ClientEntry
	{
		ClientEntry(ClientEntry& other) = delete;
		void operator=(ClientEntry& other) = delete;

	public:
		ClientEntry() = default;

		ClientEntry(std::string name, ClientSocketTCP<NetworkMessage>&& connection) :
			_name(name),
			_connection(std::move(connection))
		{}

		ClientEntry(ClientEntry&& other) noexcept
		{
			_name = other._name;

			_connection = std::move(other._connection);
		}

		void operator=(ClientEntry&& other)
		{
			if (nullptr == *this)
			{
				_name = std::move(other._name);
				_connection = std::move(other._connection);
			}
			else
				throw std::exception("Nope");//??? really?
		}


		NetworkMessage* TryReceive(int timeout)
		{
			return _connection.TryReceive(timeout);
		}

		void Send(NetworkMessage msg)
		{
			_connection.Send(msg);
		}

		std::string Name() const
		{
			return _name;
		}


		friend bool operator==(const std::nullptr_t& null, const ClientEntry& rhs)
		{
			return nullptr == rhs._connection;
		}

		friend bool operator!=(const std::nullptr_t& null, const ClientEntry& rhs)
		{
			return !(null == rhs);
		}


	private:
		std::string _name;

		ClientSocketTCP<NetworkMessage> _connection;
	};
}
