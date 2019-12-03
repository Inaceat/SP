#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include "stdafx.h"


#include "Tasks.hpp"


struct SocketDeleter
{
	void operator()(SOCKET* sPtr) const
	{
		std::cout << *sPtr << " closed" << std::endl;
		closesocket(*sPtr);
		delete sPtr;
	}
};


void Server()
{
	//Create
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == serverSocket)
	{
		std::cout << "[S]Socket creation failed: " << WSAGetLastError() << std::endl;//TODO gla ?
		return;
	}

	//Set async mode
	unsigned long mode = 1;
	ioctlsocket(serverSocket, FIONBIO, &mode);

	//Bind
	sockaddr_in addressInfo;
	ZeroMemory(&addressInfo, sizeof(sockaddr_in));

	addressInfo.sin_family = AF_INET;
	addressInfo.sin_port = htons(42042);
	addressInfo.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	auto bindResult = bind(serverSocket, reinterpret_cast<sockaddr*>(&addressInfo), sizeof(sockaddr));
	if(0 != bindResult)
	{
		std::cout << "[S]Socket binding failed: " << WSAGetLastError() << std::endl;
		return;
	}

	//Start listening
	auto listenResult = listen(serverSocket, SOMAXCONN);
	if (0 != listenResult)
	{
		std::cout << "[S]Socket listening failed: " << WSAGetLastError() << std::endl;
		return;
	}

	//Connect
	sockaddr_in clientAddressInfo;
	int clientAddressInfoSize = sizeof(sockaddr_in);
	
	ZeroMemory(&clientAddressInfo, clientAddressInfoSize);

	SOCKET clientConnectionSocket;
	while (true)
	{
		clientConnectionSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&addressInfo), &clientAddressInfoSize);
		if (INVALID_SOCKET == clientConnectionSocket)
		{
			if (WSAEWOULDBLOCK == GetLastError())
			{
				std::cout << "[S]Waiting for clients..." << std::endl;
				Sleep(1000);
				continue;
			}
			std::cout << "[S]Client connection failed: " << WSAGetLastError() << std::endl;
			return;
		}
		
		break;
	}

	//Receive
	const int bufferSize = 14;
	char buffer[bufferSize];

	auto readBytes = recv(clientConnectionSocket, buffer, bufferSize, 0);
	if (bufferSize != readBytes)
	{
		switch (readBytes)
		{
			case 0:
			{
				std::cout << "[S]Receiving error: client connection closed, " << WSAGetLastError() << std::endl;
			}break;


			case SOCKET_ERROR:
			{
				std::cout << "[S]Receiving error: " << WSAGetLastError() << std::endl;
			}break;


			default:
			{
				std::cout << "[S]Receiving error: read " << readBytes << " bytes instead of " << bufferSize << std::endl;
			}break;
		}
	}
	else
	{
		//Send
		auto bytesSent = send(clientConnectionSocket, buffer, bufferSize, 0);
		if (bufferSize != bytesSent)
		{
			if (SOCKET_ERROR == bytesSent)
				std::cout << "[S]Sending error: " << WSAGetLastError() << std::endl;
			else
				std::cout << "[S]Sending error: sent " << bytesSent << " bytes instead of " << bufferSize << std::endl;
		}
	}
	
	//Close
	shutdown(clientConnectionSocket, SD_BOTH);
	closesocket(clientConnectionSocket);

	closesocket(serverSocket);
}


void Client()
{
	std::unique_ptr<SOCKET, SocketDeleter> sockPtr(new SOCKET);

	*sockPtr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//Create
	//SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == *sockPtr)
	{
		std::cout << "[C]Socket creation failed: " << WSAGetLastError() << std::endl;//TODO gla ?
		return;
	}



	//Connect
	sockaddr_in addressInfo;
	ZeroMemory(&addressInfo, sizeof(sockaddr_in));

	addressInfo.sin_family = AF_INET;
	addressInfo.sin_port = htons(42042);
	addressInfo.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	Sleep(10000);
	
	auto connectResult = connect(*sockPtr, reinterpret_cast<sockaddr*>(&addressInfo), sizeof(sockaddr));
	if (0 != connectResult)
	{
		std::cout << "[C]Socket connection failed: " << WSAGetLastError() << std::endl;
		return;
	}

	//Read
	std::string request = "Hello, World!";
	//std::cin >> request;

	//Send
	auto bytesSent = send(*sockPtr, request.c_str(), request.size() + 1, 0);
	if (request.size() + 1 != bytesSent)
	{
		if (SOCKET_ERROR == bytesSent)
			std::cout << "[C]Sending error: " << WSAGetLastError() << std::endl;
		else
			std::cout << "[C]Sending error: sent " << bytesSent << " bytes instead of " << request.size() + 1 << std::endl;
	}
	else
	{
		//Receive
		const int bufferSize = 14;
		char buffer[bufferSize];

		auto readBytes = recv(*sockPtr, buffer, bufferSize, 0);
		if (bufferSize != readBytes)
		{
			switch (readBytes)
			{
				case 0:
				{
					std::cout << "[C]Receiving error: client connection closed, " << WSAGetLastError() << std::endl;
				}break;


				case SOCKET_ERROR:
				{
					std::cout << "[C]Receiving error: " << WSAGetLastError() << std::endl;
				}break;


				default:
				{
					std::cout << "[C]Receiving error: read " << readBytes << " bytes instead of " << bufferSize << std::endl;
				}break;
			}
		}

		//Show
		std::string answer(buffer);
		std::cout << answer << std::endl;
	}


	//Close
	shutdown(*sockPtr, SD_BOTH);
}


void Task1::Do()
{
	WSADATA startupData;
	auto startupResult = WSAStartup(MAKEWORD(2, 2), &startupData);
	if (0 != startupResult)
	{
		std::cout << "WinSock failed to init: " << startupResult << std::endl;
		return;
	}

	std::thread server(Server);


	Client();


	server.join();

	WSACleanup();

	std::cout << "Here TTT will be one day" << std::endl;
}
