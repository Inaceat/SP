#pragma once


#include <WinSock2.h>


class TextMessage
{
public:
	TextMessage(std::string text) :
		_text(text)
	{}


	char* GetMessageBytes(size_t* bytesSize)
	{
		*bytesSize = _text.size() + 1;

		char* bytes = new char[*bytesSize];

		strcpy_s(bytes, *bytesSize, _text.c_str());

		return bytes;
	}

	static TextMessage Create(char* bytes, int bytesSize)
	{
		return TextMessage(bytes, bytesSize);
	}

private:
	TextMessage(char* bytes, int bytesSize) :
		_text(bytes, bytesSize)
	{}


private:
	std::string _text;
};


template<typename TMessage>
class ClientSocketTCP
{
private:
	ClientSocketTCP(ClientSocketTCP& other) = delete;//TODO wupwupwupwupwupwup
	void operator=(ClientSocketTCP& other) = delete;

public:
	//Address should be formatted as "IP1.IP2.IP3.IP4:PORT"
	ClientSocketTCP(std::string address) :
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
		size_t messageLength;
		
		char* messageBytes = message.GetMessageBytes(&messageLength);


		int sizeToSend = sizeof(sizeToSend) + messageLength;

		char* bufferToSend = new char[sizeToSend];
		
		memcpy_s(bufferToSend, sizeToSend, &sizeToSend, sizeof(sizeToSend));

		memcpy_s(bufferToSend + sizeof(sizeToSend), messageLength, messageBytes, messageLength);

		delete[] messageBytes;

		
		int bytesSent = 0;

		while (sizeToSend > 0)
		{
			int thisTimeBytesSent = send(*_socket, bufferToSend + bytesSent, sizeToSend, 0);

			bytesSent += thisTimeBytesSent;
			sizeToSend -= thisTimeBytesSent;
		}

		delete[] bufferToSend;
	}

	TMessage Receive(int timeout)
	{
		
	}

private:
	SOCKET* _socket;
};


template<typename TMessage>
class ServerSocketTCP
{
public:
	//Address should be formatted as "IP1.IP2.IP3.IP4:PORT"
	ServerSocketTCP(std::string address)
	{
		
	}

	~ServerSocketTCP()
	{
		
	}


	ClientSocketTCP<TMessage> AcceptIncomingConnection(int timeout)
	{
		
	}

private:
	
};
