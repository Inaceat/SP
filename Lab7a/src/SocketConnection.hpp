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



template<typename TMessage>
class ServerSocketTCP;


template<typename TMessage>
class ClientSocketTCP
{
private:
	ClientSocketTCP(SOCKET socket) :
		_socket(new SOCKET)
	{
		*_socket = socket;
	}

	ClientSocketTCP(ClientSocketTCP& other) = delete;//TODO wupwupwupwupwupwup
	void operator=(ClientSocketTCP& other) = delete;

public:
	//Address should be formatted as "IP1.IP2.IP3.IP4:PORT"
	explicit ClientSocketTCP(std::string address) :
		_socket(new SOCKET)
	{
		//TODO validate {address}?
		//TODO handle errors
		//Create
		*_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
		//Connect
		sockaddr_in addressInfo;
		ZeroMemory(&addressInfo, sizeof(sockaddr_in));

		addressInfo.sin_family = AF_INET;
		addressInfo.sin_port = htons(atoi(address.substr(address.find(":") + 1, std::string::npos).c_str()));
		addressInfo.sin_addr.S_un.S_addr = inet_addr(address.substr(0, address.find(":")).c_str());

		if (0 != connect(*_socket, reinterpret_cast<sockaddr*>(&addressInfo), sizeof(sockaddr)))
			std::cout << "[C]Socket connection failed: " << WSAGetLastError() << std::endl;
	}

	ClientSocketTCP(ClientSocketTCP&& other) :
		_socket(new SOCKET)
	{
		*_socket = *(other._socket);

		other._socket = nullptr;
	}


	~ClientSocketTCP()
	{
		if (nullptr != _socket)
		{
			shutdown(*_socket, SD_BOTH);
			closesocket(*_socket);

			delete _socket;
		}
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
			int thisTimeBytesSent = send(*_socket, bufferToSend + bytesSent, bytesToSend, 0);

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
			int thisTimeBytesReceived = recv(*_socket, lengthBuffer + bytesReceived, lengthBufferSize - bytesReceived, 0);
			
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
			int thisTimeBytesReceived = recv(*_socket, messageBuffer + bytesReceived, messageLength - bytesReceived, 0);

			bytesReceived += thisTimeBytesReceived;
		}
		while (bytesReceived < messageLength);


		TMessage* result = new TMessage(messageBuffer, messageLength);

		delete[] messageBuffer;

		return result;
	}


	friend ClientSocketTCP<TMessage>* ServerSocketTCP<TMessage>::TryAcceptIncomingConnection(int timeout);

private:
	SOCKET* _socket;
};


template<typename TMessage>
class ServerSocketTCP
{
public:
	//Address should be formatted as "IP1.IP2.IP3.IP4:PORT"
	explicit ServerSocketTCP(std::string address) :
		_socket(new SOCKET)
	{
		//TODO validate {address}?
		//TODO handle errors
		//Create
		*_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		//Bind
		sockaddr_in addressInfo;
		ZeroMemory(&addressInfo, sizeof(sockaddr_in));

		addressInfo.sin_family = AF_INET;
		addressInfo.sin_port = htons(atoi(address.substr(address.find(":") + 1, std::string::npos).c_str()));
		addressInfo.sin_addr.S_un.S_addr = inet_addr(address.substr(0, address.find(":")).c_str());

		auto bindResult = bind(*_socket, reinterpret_cast<sockaddr*>(&addressInfo), sizeof(sockaddr));

		//Set non-blocking mode
		u_long mode = 1;
		ioctlsocket(*_socket, FIONBIO, &mode);

		//Start listening
		auto listenResult = listen(*_socket, SOMAXCONN);
		//if (0 != listenResult)
		//{
		//	std::cout << "[S]Socket listening failed: " << WSAGetLastError() << std::endl;
		//	return;
		//}
	}

	~ServerSocketTCP()
	{
		closesocket(*_socket);

		delete _socket;
	}


	ClientSocketTCP<TMessage>* TryAcceptIncomingConnection(int timeout)
	{
		//Client info
		sockaddr_in clientAddressInfo;
		int clientAddressInfoSize = sizeof(sockaddr_in);

		ZeroMemory(&clientAddressInfo, clientAddressInfoSize);

		//Try
		SOCKET clientConnectionSocket = accept(*_socket, reinterpret_cast<sockaddr*>(&clientAddressInfo), &clientAddressInfoSize);
		
		if (INVALID_SOCKET == clientConnectionSocket && WSAEWOULDBLOCK == GetLastError())
			Sleep(timeout);

		//Try again
		clientConnectionSocket = accept(*_socket, reinterpret_cast<sockaddr*>(&clientAddressInfo), &clientAddressInfoSize);

		if (INVALID_SOCKET == clientConnectionSocket)
			return nullptr;

		return new ClientSocketTCP<TMessage>(clientConnectionSocket);
	}

private:
	SOCKET* _socket;
};
