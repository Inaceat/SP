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
		SECURITY_DESCRIPTOR sd;
		InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(&sd, TRUE, nullptr, FALSE);



		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = &sd;
		sa.bInheritHandle = TRUE;

		_pipeHandle = CreateNamedPipe(
			name.c_str(), 
			PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
			PIPE_UNLIMITED_INSTANCES,
			1024,
			1024,
			0,
			&sa);


	}

	bool WaitConnection(int timeout)
	{
		bool result = false;

		OVERLAPPED overlapInfo;
		ZeroMemory(&overlapInfo, sizeof(OVERLAPPED));

		overlapInfo.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);

		auto connectionResult = ConnectNamedPipe(_pipeHandle, &overlapInfo);
		std::cout << "ConnectNamedPipe returned " << connectionResult << ", err == " << GetLastError() << std::endl;

		if (FALSE == connectionResult && ERROR_IO_PENDING == GetLastError())
		{
			DWORD waitResult = WaitForSingleObject(overlapInfo.hEvent, timeout);

			std::cout << "ConnectNamedPipe wait resulted in " << waitResult << ", err == " << GetLastError() << std::endl;

			if (WAIT_TIMEOUT == waitResult)
				CancelIo(_pipeHandle);

			if (WAIT_OBJECT_0 == waitResult)
				result = true;
		}

		if (FALSE == connectionResult && ERROR_PIPE_CONNECTED == GetLastError())
			result = true;

		CloseHandle(overlapInfo.hEvent);

		return result;
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

		PeekNamedPipe(_pipeHandle, nullptr, 0, nullptr, nullptr, &nextMessageSize);
		
		if(0 == nextMessageSize)
			return nullptr;

		char* messageBuffer = new char[nextMessageSize];
		DWORD readBytes;


		OVERLAPPED overlapInfo;
		ZeroMemory(&overlapInfo, sizeof(OVERLAPPED));
		overlapInfo.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);


		ReadFile(_pipeHandle, messageBuffer, nextMessageSize, &readBytes, &overlapInfo);
		
		WaitForSingleObject(overlapInfo.hEvent, INFINITE);


		TMessage* result = TMessage::Create(messageBuffer, readBytes);

		CloseHandle(overlapInfo.hEvent);

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
		_name = name;

		_pipeHandle = CreateFile(name.c_str(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);

		if (INVALID_HANDLE_VALUE == _pipeHandle)
			std::cout << "ClientPipe creation failed with error " << GetLastError() << std::endl;
	}

	~ClientSideNamedPipeConnection()
	{
		CloseHandle(_pipeHandle);
	}


	std::string Name() const
	{
		return _name;
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
	std::string _name;

	HANDLE _pipeHandle;
};