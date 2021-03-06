#include "stdafx.h"


#include "UserMessage.hpp"


namespace Chateg
{
	UserMessage::UserMessage(std::string senderName, std::string text):
		_senderName(senderName),
		_text(text)
	{}


	std::string UserMessage::SenderName() const
	{
		return _senderName;
	}

	std::string UserMessage::Text() const
	{
		return _text;
	}


	char* UserMessage::GetMessageBytes(int* messageLength)
	{
		*messageLength = _senderName.size() + 1 + _text.size() + 1;

		char* bytes = new char[*messageLength];


		memcpy_s(bytes, _senderName.size() + 1, _senderName.c_str(), _senderName.size() + 1);

		memcpy_s(bytes + _senderName.size() + 1, _text.size() + 1, _text.c_str(), _text.size() + 1);


		return bytes;
	}

	UserMessage* UserMessage::Create(char* bytes, int bytesSize)
	{
		return new UserMessage(bytes, bytesSize);
	}


	UserMessage::UserMessage(char* bytes, int bytesSize)
	{
		_senderName = std::string(bytes);

		_text = std::string(bytes + _senderName.size() + 1);
	}
}
