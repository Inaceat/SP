#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include "stdafx.h"


#include "Tasks.hpp"


#include "SocketConnection.hpp"


void Server()
{
	ServerSocketTCP<TextMessage> serverSocket("127.0.0.1:42042");

	ClientSocketTCP<TextMessage> clientSocket = serverSocket.TryAcceptIncomingConnection(1000);
	if (nullptr != clientSocket)
	{
		TextMessage* msg = clientSocket.TryReceive(0);
		
		clientSocket.Send(*msg);
		
		delete msg;
		
		
		Sleep(5000);
	}
	else
		std::cout << "No clients!" << std::endl;
	

	//Receive
	//const int bufferSize = 18;
	//char buffer[bufferSize];
	//
	//auto readBytes = recv(clientConnectionSocket, buffer, bufferSize, 0);
	//if (bufferSize != readBytes)
	//{
	//	switch (readBytes)
	//	{
	//		case 0:
	//		{
	//			std::cout << "[S]Receiving error: client connection closed, " << WSAGetLastError() << std::endl;
	//		}break;
	//
	//
	//		case SOCKET_ERROR:
	//		{
	//			std::cout << "[S]Receiving error: " << WSAGetLastError() << std::endl;
	//		}break;
	//
	//
	//		default:
	//		{
	//			std::cout << "[S]Receiving error: read " << readBytes << " bytes instead of " << bufferSize << std::endl;
	//		}break;
	//	}
	//}
	//else
	//{
	//	//Send
	//	auto bytesSent = send(clientConnectionSocket, buffer, bufferSize, 0);
	//	if (bufferSize != bytesSent)
	//	{
	//		if (SOCKET_ERROR == bytesSent)
	//			std::cout << "[S]Sending error: " << WSAGetLastError() << std::endl;
	//		else
	//			std::cout << "[S]Sending error: sent " << bytesSent << " bytes instead of " << bufferSize << std::endl;
	//	}
	//}
	//Sleep(10000);
	////Close
	//shutdown(clientConnectionSocket, SD_BOTH);
	//closesocket(clientConnectionSocket);
}


void Client()
{
	Sleep(500);

	ClientSocketTCP<TextMessage> s("127.0.0.1:42042");

	Sleep(2000);

	TextMessage request("1234");
	s.Send(request);

	Sleep(1000);

	TextMessage* answer = s.TryReceive(0);
	std::cout << answer->Text() << std::endl;
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
