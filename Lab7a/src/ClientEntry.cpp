#include "stdafx.h"


#include "ClientEntry.hpp"


namespace TTT
{
	ClientEntry::ClientEntry(std::string name, ClientSocketTCP<NetworkMessage>&& connection):
		_name(name),
		_connection(std::move(connection))
	{
	}


	ClientEntry::ClientEntry(ClientEntry&& other) noexcept
	{
		_name = other._name;

		_connection = std::move(other._connection);
	}

	void ClientEntry::operator=(ClientEntry&& other)
	{
		if (nullptr == *this)
		{
			_name = std::move(other._name);
			_connection = std::move(other._connection);
		}
		else
			throw std::exception("Nope"); //??? really?
	}


	NetworkMessage* ClientEntry::TryReceive(int timeout)
	{
		return _connection.TryReceive(timeout);
	}

	void ClientEntry::Send(NetworkMessage msg)
	{
		_connection.Send(msg);
	}


	std::string ClientEntry::Name() const
	{
		return _name;
	}


	bool operator==(const std::nullptr_t& null, const ClientEntry& rhs)
	{
		return nullptr == rhs._connection;
	}

	bool operator!=(const std::nullptr_t& null, const ClientEntry& rhs)
	{
		return !(null == rhs);
	}
}
