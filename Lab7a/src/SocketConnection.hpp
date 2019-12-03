#pragma once


#include <WinSock2.h>


struct SocketDeleter
{
	void operator()(SOCKET* sPtr) const
	{
		closesocket(*sPtr);

		delete sPtr;
	}
};


template<typename TMessage>
class ClientSocketTCP
{
public:
	//Address should be formatted as "IP1.IP2.IP3.IP4:PORT"
	ClientSocketTCP(std::string address)
	{
		//Create
		*_socketPtr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (INVALID_SOCKET == *_socketPtr)
		{
			std::cout << "[C]Socket creation failed: " << WSAGetLastError() << std::endl;
			return;//TODO mb throw?
		}



		//Connect
		sockaddr_in addressInfo;
		ZeroMemory(&addressInfo, sizeof(sockaddr_in));

		addressInfo.sin_family = AF_INET;
		addressInfo.sin_port = htons(atoi(address.substr(address.find(":"), std::string::npos).c_str()));//TODO debug this
		addressInfo.sin_addr.S_un.S_addr = inet_addr(address.substr(0, address.find(":")).c_str());
	}

	~ClientSocketTCP()
	{

	}


	void Send(TMessage message)
	{
		
	}

	TMessage Receive(int timeout)
	{
		
	}

private:
	std::unique_ptr<SOCKET, SocketDeleter> _socketPtr;
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
