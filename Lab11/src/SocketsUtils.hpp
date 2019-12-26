#pragma once


#include <WinSock2.h>
#include <WS2tcpip.h>


class SocketDeleter
{
public:
	void operator()(SOCKET* s);
};


class TextMessage
{
public:
	TextMessage(std::string text);


	TextMessage(char* bytes, int bytesSize);

	char* GetMessageBytes(int* bytesSize);


	std::string Text() const;


private:
	std::string _text;
};
