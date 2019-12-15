#pragma once


#include <WinSock2.h>
#include <WS2tcpip.h>


class SocketDeleter
{
public:
	void operator()(SOCKET* s)
	{
		if (nullptr != s)
		{
			std::cout << ", " << *s << std::endl;
			closesocket(*s);

			delete s;
		}
	}
};


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
