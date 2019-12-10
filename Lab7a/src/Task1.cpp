#include "stdafx.h"


#include "Tasks.hpp"


#include "SocketsTCP.hpp"
#include "SocketsUDP.hpp"
#include "Client.hpp"

/*
void ServerUDP()
{
	BroadcastReceiverSocketUDP<TextMessage> serverSocket("42042");
	
	sockaddr_in sender;
	
	while (true)
	{
		TextMessage* result = serverSocket.TryReceive(100, sender);
	
		if (nullptr != result)
		{
			int bufsize = 100;
			char* buf = new char[bufsize];
	
			inet_ntop(AF_INET, &(sender.sin_addr), buf, bufsize);
	
			std::cout << "Received " << result->Text() << " from " << std::string(buf) << std::endl;
	
			delete result;
			delete[] buf;
			
			break;
		}
		else
			std::cout << " Not received" << std::endl;
	}
	
	Sleep(5000);
}

void ClientUDP()
{
	Sleep(2000);

	BroadcasSendertSocketUDP<TextMessage> s("10.1.19.255:42042");


	TextMessage request("1234");

	s.Send(request);

	Sleep(4000);
}



void ServerTCP()
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

void ClientTCP()
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
*/

void Task1::Do()
{
	/*WSADATA startupData;
	auto startupResult = WSAStartup(MAKEWORD(2, 2), &startupData);
	if (0 != startupResult)
	{
		std::cout << "WinSock failed to init: " << startupResult << std::endl;
		return;
	}
	
	std::thread server(ServerUDP);
	
	
	ClientUDP();
	
	
	server.join();
	
	WSACleanup();


	std::cout << "Here TTT will be one day" << std::endl;*/

	TTT::Client ticTackToeClient;

	ticTackToeClient.Start();
}
