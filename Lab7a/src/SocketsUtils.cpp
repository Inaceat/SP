#include "stdafx.h"


#include "SocketsUtils.hpp"


void SocketDeleter::operator()(SOCKET* s)
{
	if (nullptr != s)
	{
		closesocket(*s);

		delete s;
	}
}



TextMessage::TextMessage(std::string text):
	_text(text)
{
}


TextMessage::TextMessage(char* bytes, int bytesSize):
	_text(bytes)
{
}

char* TextMessage::GetMessageBytes(int* bytesSize)
{
	*bytesSize = static_cast<int>(_text.size()) + 1;

	char* bytes = new char[*bytesSize];

	strcpy_s(bytes, *bytesSize, _text.c_str());

	return bytes;
}


std::string TextMessage::Text() const
{
	return _text;
}
