#pragma once


template<typename TMessage>
class ServerSideNamedPipeConnection
{
private:
	ServerSideNamedPipeConnection(const ServerSideNamedPipeConnection&) = delete;
	ServerSideNamedPipeConnection& operator=(const ServerSideNamedPipeConnection&) = delete;

public:
	ServerSideNamedPipeConnection(std::string name)
	{
		_pipeHandle = CreateNamedPipe(
			name.c_str(), 
			PIPE_ACCESS_INBOUND, //TODO FILE_FLAG_FIRST_PIPE_INSTANCE for same name? FILE_FLAG_OVERLAPPED?
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
			PIPE_UNLIMITED_INSTANCES,
			1024,
			1024,
			0,
			nullptr);
	}

	~ServerSideNamedPipeConnection()
	{
		CloseHandle();
	}


	TMessage* MessageReceive()
	{
		char* messageBuffer = new char[nextMessageSize];
		DWORD readBytes;

		bool readSuccessful = ReadFile(_mailslotHandle, messageBuffer, nextMessageSize, &readBytes, nullptr);

		if (!readSuccessful)
		{
			delete[] messageBuffer;

			return nullptr;
		}

		TMessage* result = TMessage::Create(messageBuffer, readBytes);

		delete[] messageBuffer;

		return result;
	}


private:
	HANDLE _pipeHandle;
};


template<typename TMessage>
class ClientSideNamedPipeConnection
{
private:
	ClientSideNamedPipeConnection(const ClientSideNamedPipeConnection&) = delete;
	ClientSideNamedPipeConnection& operator=(const ClientSideNamedPipeConnection&) = delete;

public:
	ClientSideNamedPipeConnection(std::string name)
	{
		_pipeHandle = ;
	}

	~ClientSideNamedPipeConnection()
	{
		CloseHandle();
	}


	void MessageSend(const TMessage* message)
	{
		int messageLength;
		char* messageBytes = message->GetMessageBytes(&messageLength);

		DWORD bytesWritten;

		//WriteFile(_mailslotHandle, messageBytes, messageLength, &bytesWritten, nullptr);

		delete[] messageBytes;
	}


private:
	HANDLE _pipeHandle;
};