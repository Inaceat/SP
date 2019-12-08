#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include "stdafx.h"


#include "Tasks.hpp"


#include "SocketConnection.hpp"


void Server()
{
	ServerSocketTCP<TextMessage> serverSocket("127.0.0.1:42042");

	ClientSocketTCP<TextMessage> clientSocket;

	while (true)
	{
		clientSocket = serverSocket.TryAcceptIncomingConnection(100);

		if (nullptr != clientSocket)
			break;
	}

	std::cout << "[S]connected" << std::endl;

	while (true)
	{
		TextMessage* msg = clientSocket.TryReceive(100);

		if (nullptr != msg)
		{
			std::cout << "[S]received " << msg->Text() << std::endl;

			clientSocket.Send(*msg);
	
			delete msg;
			
			Sleep(5000);

			break;
		}
	}
}


void Client()
{
	Sleep(2000);

	ClientSocketTCP<TextMessage> s("127.0.0.1:42042");


	TextMessage request("1234");

	Sleep(2000);

	s.Send(request);


	while (true)
	{
		TextMessage* answer = s.TryReceive(100);

		if (nullptr != answer)
		{
			std::cout << answer->Text() << std::endl;
	
			delete answer;

			break;
		}
	}
	



	
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
