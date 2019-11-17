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

	bool WaitConnection(int timeout)
	{
		OVERLAPPED overlapInfo;
		ZeroMemory(&overlapInfo, sizeof(OVERLAPPED));

		overlapInfo.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);

		ConnectNamedPipe(_pipeHandle, &overlapInfo);


		DWORD waitResult = WaitForSingleObject(overlapInfo.hEvent, timeout);


		if (WAIT_TIMEOUT != waitResult)
			CancelIo(_pipeHandle);//TODO maybe check

		CloseHandle(overlapInfo.hEvent);

		return WAIT_OBJECT_0 == waitResult;
	}


	~ServerSideNamedPipeConnection()
	{
		DisconnectNamedPipe(_pipeHandle);
		CloseHandle(_pipeHandle);
	}


	bool HasMessages() const
	{
		DWORD nextMessageSize;

		if (TRUE == PeekNamedPipe(_pipeHandle, nullptr, 0, nullptr, nullptr, &nextMessageSize) && 0 != nextMessageSize)
			return true;

		return false;
		
	}

	TMessage* MessageReceive()
	{
		DWORD nextMessageSize;

		PeekNamedPipe(_pipeHandle, nullptr, 0, nullptr, nullptr, &nextMessageSize);//TODO check this
		
		if(0 == nextMessageSize)
			return nullptr;

		char* messageBuffer = new char[nextMessageSize];
		DWORD readBytes;

		bool readSuccessful = ReadFile(_pipeHandle, messageBuffer, nextMessageSize, &readBytes, nullptr);

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
		_pipeHandle = CreateFile(name.c_str(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	}

	~ClientSideNamedPipeConnection()
	{
		CloseHandle(_pipeHandle);
	}


	void MessageSend(const TMessage* message)
	{
		int messageLength;
		char* messageBytes = message->GetMessageBytes(&messageLength);

		DWORD bytesWritten;

		WriteFile(_pipeHandle, messageBytes, messageLength, &bytesWritten, nullptr);

		delete[] messageBytes;
	}


private:
	HANDLE _pipeHandle;
};