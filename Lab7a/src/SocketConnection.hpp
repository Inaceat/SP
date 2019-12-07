#pragma once


#include <WinSock2.h>


class TextMessage
{
public:
	TextMessage(std::string text) :
		_text(text)
	{}


	TextMessage(char* bytes, int bytesSize) :
		_text(bytes)
	{}

	char* GetMessageBytes(int* bytesSize)
	{
		*bytesSize = static_cast<int>(_text.size()) + 1;

		char* bytes = new char[*bytesSize];

		strcpy_s(bytes, *bytesSize, _text.c_str());

		return bytes;
	}


	std::string Text() const
	{
		return _text;
	}


private:
	std::string _text;
};

class SocketDeleter
	{
	public:
		void operator()(SOCKET* s)
		{
			std::cout << "deleting S " << s;
			if (nullptr != s)
			{
				std::cout << ", " << *s << std::endl;
				closesocket(*s);

				delete s;
			}
		}
	};


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
		addressInfo.sin_addr.S_un.S_addr = inet_addr(address.substr(0, address.find(":")).c_str());


		if (0 != connect(newSocket, reinterpret_cast<sockaddr*>(&addressInfo), sizeof(sockaddr)))
		{
			closesocket(newSocket);
			return;
		}


		_socketPtr.reset(new SOCKET(newSocket));
	}

	ClientSocketTCP(ClientSocketTCP&& other) noexcept
	{
		//TODO check
		_socketPtr = other._socketPtr;
	}


	~ClientSocketTCP()
	{
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

			bytesSent += thisTimeBytesSent;
			bytesToSend -= thisTimeBytesSent;
		}

		delete[] bufferToSend;
	}

	TMessage* TryReceive(int timeout)
	{
		//Get message length
		int messageLength;

		int lengthBufferSize = sizeof(messageLength);
		char* lengthBuffer = new char[lengthBufferSize];

		int bytesReceived = 0;
		do
		{
			int thisTimeBytesReceived = recv(*_socketPtr, lengthBuffer + bytesReceived, lengthBufferSize - bytesReceived, 0);
			
			bytesReceived += thisTimeBytesReceived;
		}
		while (bytesReceived < lengthBufferSize);
		

		memcpy_s(&messageLength, sizeof(messageLength), lengthBuffer, lengthBufferSize);

		delete[] lengthBuffer;


		//Get message
		char* messageBuffer = new char[messageLength];

		bytesReceived = 0;
		do
		{
			int thisTimeBytesReceived = recv(*_socketPtr, messageBuffer + bytesReceived, messageLength - bytesReceived, 0);

			bytesReceived += thisTimeBytesReceived;
		}
		while (bytesReceived < messageLength);


		TMessage* result = new TMessage(messageBuffer, messageLength);

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
		//TODO validate {address}?
		
		//Create
		SOCKET newSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (INVALID_SOCKET == newSocket)
			return;


		//Bind
		sockaddr_in addressInfo;
		ZeroMemory(&addressInfo, sizeof(sockaddr_in));

		addressInfo.sin_family = AF_INET;
		addressInfo.sin_port = htons(atoi(address.substr(address.find(":") + 1, std::string::npos).c_str()));
		addressInfo.sin_addr.S_un.S_addr = inet_addr(address.substr(0, address.find(":")).c_str());


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

		//Try to accept
		SOCKET clientConnectionSocket = accept(*_socketPtr, reinterpret_cast<sockaddr*>(&clientAddressInfo), &clientAddressInfoSize);

		//If succeed
		if (INVALID_SOCKET != clientConnectionSocket)
		{
			return ClientSocketTCP<TMessage>(clientConnectionSocket);
		}

		//If failed & not cause of pending operation
		if(WSAEWOULDBLOCK != GetLastError())
		{
			shutdown(*_socketPtr, SD_BOTH);
			_socketPtr.reset(nullptr);

			return ClientSocketTCP<TMessage>(INVALID_SOCKET);
		}

		//Now waiting for incoming connections
		timeval time;
		time.tv_sec = 0;
		time.tv_usec = timeout * 1000;//timeout is ms, so to get us we multiply by 1000

		fd_set set;
		FD_ZERO(&set);
		FD_SET(*_socketPtr, &set);

		int readySocketsCount = select(9001, &set, nullptr, nullptr, &time);//First param is ignored

		//If error occured
		if(SOCKET_ERROR == readySocketsCount)
		{
			shutdown(*_socketPtr, SD_BOTH);
			_socketPtr.reset(nullptr);

			return ClientSocketTCP<TMessage>(INVALID_SOCKET);
		}

		//If socket is not ready
		if (0 == readySocketsCount)
		{
			return ClientSocketTCP<TMessage>(INVALID_SOCKET);
		}

		//Now connection is possible
		clientConnectionSocket = accept(*_socketPtr, reinterpret_cast<sockaddr*>(&clientAddressInfo), &clientAddressInfoSize);

		return ClientSocketTCP<TMessage>(clientConnectionSocket);
	}

private:
	std::unique_ptr<SOCKET, SocketDeleter> _socketPtr;
};
