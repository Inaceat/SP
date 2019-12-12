#pragma once


#include "SocketsUtils.hpp"


template<typename TMessage>
class ServerSocketTCP;

template<typename TMessage>
class ClientSocketTCP
{
	ClientSocketTCP(SOCKET socket)
	{
		if (INVALID_SOCKET != socket)
			_socketPtr.reset(new SOCKET(socket));
	}


	ClientSocketTCP(ClientSocketTCP& other) = delete;
	void operator=(ClientSocketTCP& other) = delete;

public:
	ClientSocketTCP() = default;

	//Address should be formatted as "IP1.IP2.IP3.IP4:PORT"
	explicit ClientSocketTCP(std::string address) :
		_socketPtr(nullptr)
	{
		//TODO maybe validate {address}?
		
		//Create
		SOCKET newSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
		if (INVALID_SOCKET == newSocket)
			return;


		//Connect
		sockaddr_in addressInfo;
		ZeroMemory(&addressInfo, sizeof(sockaddr_in));

		addressInfo.sin_family = AF_INET;
		addressInfo.sin_port = htons(atoi(address.substr(address.find(":") + 1, std::string::npos).c_str()));
		inet_pton(AF_INET, address.substr(0, address.find(":")).c_str(), &(addressInfo.sin_addr));
		


		if (0 != connect(newSocket, reinterpret_cast<sockaddr*>(&addressInfo), sizeof(sockaddr)))
		{
			closesocket(newSocket);
			return;
		}


		_socketPtr.reset(new SOCKET(newSocket));
	}

	explicit ClientSocketTCP(sockaddr_in address) :
		_socketPtr(nullptr)
	{
		//TODO maybe validate {address}?
		
		//Create
		SOCKET newSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
		if (INVALID_SOCKET == newSocket)
			return;


		//Connect
		if (0 != connect(newSocket, reinterpret_cast<sockaddr*>(&address), sizeof(sockaddr)))
		{
			closesocket(newSocket);
			return;
		}


		_socketPtr.reset(new SOCKET(newSocket));
	}


	ClientSocketTCP(ClientSocketTCP&& other) noexcept
	{
		_socketPtr = other._socketPtr;
	}

	void operator=(ClientSocketTCP&& other) noexcept
	{
		_socketPtr = std::move(other._socketPtr);
	}

	~ClientSocketTCP()
	{
		if (nullptr != _socketPtr)
			shutdown(*_socketPtr, SD_BOTH);
	}


	void Send(TMessage message)
	{
		//Convert message to bytes
		int messageLength;
		char* messageBytes = message.GetMessageBytes(&messageLength);

		//Allocate buffer
		int bytesToSend = sizeof(messageLength) + messageLength;
		char* bufferToSend = new char[bytesToSend];

		//Copy message size to buffer
		memcpy_s(bufferToSend, bytesToSend, &messageLength, sizeof(messageLength));

		//Copy message to buffer
		memcpy_s(bufferToSend + sizeof(messageLength), messageLength, messageBytes, messageLength);

		delete[] messageBytes;

		//Send
		int bytesSent = 0;
		while (bytesToSend > 0)
		{
			int thisTimeBytesSent = send(*_socketPtr, bufferToSend + bytesSent, bytesToSend, 0);

			if (SOCKET_ERROR == thisTimeBytesSent)
			{
				shutdown(*_socketPtr, SD_BOTH);
				_socketPtr.reset(nullptr);

				delete[] bufferToSend;
				return;
			}

			bytesSent += thisTimeBytesSent;
			bytesToSend -= thisTimeBytesSent;
		}

		delete[] bufferToSend;
	}

	TMessage* TryReceive(int timeout)
	{
		//Prepare for waiting
		timeval time;
		time.tv_sec = timeout / 1000;			//seconds
		time.tv_usec = (timeout % 1000) * 1000;	//microseconds, timeout is 'ms', so to get 'us' we multiply by 1000

		fd_set set;
		FD_ZERO(&set);
		FD_SET(*_socketPtr, &set);


		//Get message length
		int messageLength;

		int lengthBufferSize = sizeof(messageLength);
		char* lengthBuffer = new char[lengthBufferSize];


		//Wait till data available
		int readySocketsCount = select(9001, &set, nullptr, nullptr, &time);//First param is ignored

		//If error occured, PANIC
		if(SOCKET_ERROR == readySocketsCount)
		{
			shutdown(*_socketPtr, SD_BOTH);
			_socketPtr.reset(nullptr);

			delete[] lengthBuffer;
			return nullptr;
		}

		//If socket has no data after timeout, return nothing
		if (0 == readySocketsCount)
		{
			delete[] lengthBuffer;
			return nullptr;
		}


		//Now there is some data, peek it
		int bytesReceived = recv(*_socketPtr, lengthBuffer, lengthBufferSize, MSG_PEEK);

		//If length was not read fully, return, leaving data in buffer
		if (bytesReceived != lengthBufferSize)
		{
			delete[] lengthBuffer;
			return nullptr;
		}

		//Now get length from buffer
		memcpy_s(&messageLength, sizeof(messageLength), lengthBuffer, lengthBufferSize);

		delete[] lengthBuffer;


		
		//Get message length and message itself
		char* messageBuffer = new char[messageLength + sizeof(messageLength)];

		//We've already read 'messageLength', so we have data to read, at least 'messageLength' itself
		bytesReceived = recv(*_socketPtr, messageBuffer, messageLength + sizeof(messageLength), MSG_PEEK);

		if (bytesReceived != (messageLength + sizeof(messageLength)))
		{
			delete[] messageBuffer;
			return nullptr;
		}

		//Now buffer has all data we need, so now we read it
		recv(*_socketPtr, messageBuffer, messageLength + sizeof(messageLength), 0);


		TMessage* result = new TMessage(messageBuffer + sizeof(messageLength), messageLength);

		delete[] messageBuffer;

		return result;
	}



	friend bool operator==(const std::nullptr_t&, const ClientSocketTCP& socket)
	{
		return socket._socketPtr == nullptr;
	}

	friend bool operator!=(const std::nullptr_t&, const ClientSocketTCP& socket)
	{
		return socket._socketPtr != nullptr;
	}


	friend ClientSocketTCP<TMessage> ServerSocketTCP<TMessage>::TryAcceptIncomingConnection(int timeout);

private:
	std::unique_ptr<SOCKET, SocketDeleter> _socketPtr;
};


template<typename TMessage>
class ServerSocketTCP
{
public:
	//Address should be formatted as "IP1.IP2.IP3.IP4:PORT"
	explicit ServerSocketTCP(std::string address)
	{
		//TODO maybe validate {address}?
		
		//Create
		SOCKET newSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (INVALID_SOCKET == newSocket)
			return;


		//Bind
		sockaddr_in addressInfo;
		ZeroMemory(&addressInfo, sizeof(sockaddr_in));

		addressInfo.sin_family = AF_INET;
		addressInfo.sin_port = htons(atoi(address.substr(address.find(":") + 1, std::string::npos).c_str()));
		inet_pton(AF_INET, address.substr(0, address.find(":")).c_str(), &(addressInfo.sin_addr));


		if (0 != bind(newSocket, reinterpret_cast<sockaddr*>(&addressInfo), sizeof(sockaddr)))
		{
			closesocket(newSocket);
			return;
		}

		
		//Set non-blocking mode
		u_long mode = 1;
		ioctlsocket(newSocket, FIONBIO, &mode);


		//Start listening
		if (0 != listen(newSocket, SOMAXCONN))
		{
			closesocket(newSocket);
			return;
		}


		_socketPtr.reset(new SOCKET(newSocket));
	}

	~ServerSocketTCP()
	{
		//Set blocking mode
		u_long mode = 0;
		ioctlsocket(*_socketPtr, FIONBIO, &mode);

		
		shutdown(*_socketPtr, SD_BOTH);
	}


	ClientSocketTCP<TMessage> TryAcceptIncomingConnection(int timeout)
	{
		//Client info
		sockaddr_in clientAddressInfo;
		int clientAddressInfoSize = sizeof(sockaddr_in);

		ZeroMemory(&clientAddressInfo, clientAddressInfoSize);


		//Waiting for incoming connections
		timeval time;
		time.tv_sec = timeout / 1000;			//seconds
		time.tv_usec = (timeout % 1000) * 1000;	//microseconds, timeout is ms, so to get us we multiply by 1000

		fd_set set;
		FD_ZERO(&set);
		FD_SET(*_socketPtr, &set);

		int readySocketsCount = select(9001, &set, nullptr, nullptr, &time);//First param is ignored

		//If error occured, PANIC! 
		if(SOCKET_ERROR == readySocketsCount)
		{
			shutdown(*_socketPtr, SD_BOTH);
			_socketPtr.reset(nullptr);

			return ClientSocketTCP<TMessage>(INVALID_SOCKET);
		}

		//If socket is not ready, return smth kinda null
		if (0 == readySocketsCount)
		{
			return ClientSocketTCP<TMessage>(INVALID_SOCKET);
		}


		//Now connection is possible
		SOCKET clientConnectionSocket = accept(*_socketPtr, reinterpret_cast<sockaddr*>(&clientAddressInfo), &clientAddressInfoSize);

		//If error occured
		if (INVALID_SOCKET == clientConnectionSocket)
		{
			return ClientSocketTCP<TMessage>(INVALID_SOCKET);
		}


		//Now all's good
		return ClientSocketTCP<TMessage>(clientConnectionSocket);
	}

private:
	std::unique_ptr<SOCKET, SocketDeleter> _socketPtr;
};
