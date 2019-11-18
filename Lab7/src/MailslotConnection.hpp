#pragma once


template<typename TMessage>
class ServerSideMailslotConnection
{
private:
	ServerSideMailslotConnection(const ServerSideMailslotConnection&) = delete;
	ServerSideMailslotConnection& operator=(const ServerSideMailslotConnection&) = delete;

public:
	ServerSideMailslotConnection(std::string name)
	{
		_mailslotHandle = CreateMailslot(name.c_str(), 0, 0, nullptr);
	}

	~ServerSideMailslotConnection()
	{
		CloseHandle(_mailslotHandle);
	}


	bool HasMessages() const
	{
		DWORD nextMessageSize;

		GetMailslotInfo(_mailslotHandle, nullptr, &nextMessageSize, nullptr, nullptr);

		return MAILSLOT_NO_MESSAGE != nextMessageSize;
	}

	TMessage* MessageReceive()
	{
		DWORD nextMessageSize;

		GetMailslotInfo(_mailslotHandle, nullptr, &nextMessageSize, nullptr, nullptr);

		if (MAILSLOT_NO_MESSAGE == nextMessageSize)
			return nullptr;


		char* messageBuffer = new char[nextMessageSize];
		DWORD readBytes;

		bool readSuccessful = ReadFile(_mailslotHandle, messageBuffer, nextMessageSize, &readBytes, nullptr);

		if(!readSuccessful)
		{
			delete[] messageBuffer;

			return nullptr;
		}

		TMessage* result = TMessage::Create(messageBuffer, readBytes);

		delete[] messageBuffer;

		return result;
	}


private:
	HANDLE _mailslotHandle;
};


template<typename TMessage>
class ClientSideMailslotConnection
{
private:
	ClientSideMailslotConnection(const ClientSideMailslotConnection&) = delete;
	ClientSideMailslotConnection& operator=(const ClientSideMailslotConnection&) = delete;

public:
	ClientSideMailslotConnection(std::string name) :
		_serverName(name)
	{}

	~ClientSideMailslotConnection()
	{
		CloseHandle(_mailslotHandle);
	}


	bool TryConnect()
	{
		_mailslotHandle = CreateFile(_serverName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		return INVALID_HANDLE_VALUE != _mailslotHandle;
	}


	void MessageSend(const TMessage* message)
	{
		int messageLength;
		char* messageBytes = message->GetMessageBytes(&messageLength);

		DWORD bytesWritten;

		WriteFile(_mailslotHandle, messageBytes, messageLength, &bytesWritten, nullptr);

		delete[] messageBytes;
	}


private:
	std::string _serverName;
	HANDLE _mailslotHandle;
};