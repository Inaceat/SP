#include "stdafx.h"


#include "ChategMessage.hpp"


ChategMessage::ChategMessage(MessageType type, std::string text)
{
	_type = type;
	_text = text;
}

ChategMessage::ChategMessage(std::string text)
{
	_type = MessageType::Text;
	_text = text;
}


ChategMessage* ChategMessage::Create(char* messageBytes, int bytesSize)
{
	MessageType type;

	//TODO meh, switch
	switch (messageBytes[0])
	{
		case 0:
			type = MessageType::Text;
			break;

		case 1:
			type = MessageType::Service;
			break;

		default:
			throw "AAAAAAAAAAAAAAAAAAAAAAA";
			break;
	}

	return new ChategMessage(type, std::string(messageBytes + 1, bytesSize - 1));
}

char* ChategMessage::GetMessageBytes(int* bytesSize) const
{
	*bytesSize = (_text.size() + 2);//+1 for '\0' and +1 for MessageType

	char* messageBytes = new char[*bytesSize];

	//Type
	if (MessageType::Text == _type)
		messageBytes[0] = 0;
	else
		messageBytes[0] = 1;

	//Text
	strcpy_s(messageBytes + 1, *bytesSize - 1, _text.c_str());


	return messageBytes;
}


std::string ChategMessage::ToString() const
{
	//TODO remove
	std::string result = "";

	if (MessageType::Text == _type)
		result += "[T]";
	else
		result += "[S]";


	result += _text;

	return result;
}
