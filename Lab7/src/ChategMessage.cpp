#include "stdafx.h"


#include "ChategMessage.hpp"


ChategMessage::ChategMessage(std::string text)
{
	_text = text;
}


ChategMessage* ChategMessage::Create(char* messageBytes, int bytesSize)
{
	return new ChategMessage(std::string(messageBytes, bytesSize));
}

char* ChategMessage::GetMessageBytes(int* bytesSize) const
{
	*bytesSize = (_text.size() + 1);

	char* messageBytes = new char[*bytesSize];


	strcpy_s(messageBytes, *bytesSize, _text.c_str());


	return messageBytes;
}


std::string ChategMessage::ToString() const
{
	return _text;
}
