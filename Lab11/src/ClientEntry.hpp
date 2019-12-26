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

		ClientEntry(std::string name, ClientSocketTCP<NetworkMessage>&& connection);


		ClientEntry(ClientEntry&& other) noexcept;

		void operator=(ClientEntry&& other);


		NetworkMessage* TryReceive(int timeout);

		void Send(NetworkMessage msg);


		std::string Name() const;


		friend bool operator==(const std::nullptr_t& null, const ClientEntry& rhs);

		friend bool operator!=(const std::nullptr_t& null, const ClientEntry& rhs);


	private:
		std::string _name;

		ClientSocketTCP<NetworkMessage> _connection;
	};
}
