#include "stdafx.h"


#include "ChategNetworkMessage.hpp"


namespace Chateg
{
	ChategNetworkMessage::ChategNetworkMessage(MessageType type, std::string text)
	{
		_type = type;
		_text = text;
	}
	
	ChategNetworkMessage::ChategNetworkMessage(std::string text)
	{
		_type = MessageType::Text;
		_text = text;
	}
	
	
	ChategNetworkMessage* ChategNetworkMessage::Create(char* messageBytes, int bytesSize)
	{
		MessageType type;
	
		switch (messageBytes[0])
		{
			case 0:
				type = MessageType::Text;
				break;
	
			case 1:
				type = MessageType::Register;
				break;

			case 2:
				type = MessageType::Unregister;
				break;

			default:
				throw "AAAAAAAAAAAAAAAAAAAAAAA";
		}
	
		return new ChategNetworkMessage(type, std::string(messageBytes + 1));
	}
	
	char* ChategNetworkMessage::GetMessageBytes(int* bytesSize) const
	{
		*bytesSize = (_text.size() + 2);//+1 for '\0' and +1 for MessageType
	
		char* messageBytes = new char[*bytesSize];
	
		//Type
		switch (_type)
		{
			case MessageType::Text: 
				messageBytes[0] = 0; 
				break;

			case MessageType::Register:
				messageBytes[0] = 1;
				break;

			case MessageType::Unregister:
				messageBytes[0] = 2; 
				break;
		}

		//Text
		strcpy_s(messageBytes + 1, *bytesSize - 1, _text.c_str());
	
	
		return messageBytes;
	}


	ChategNetworkMessage::MessageType ChategNetworkMessage::Type() const
	{
		return _type;
	}

	std::string ChategNetworkMessage::Data() const
	{
		return _text;
	}
	
	
	std::string ChategNetworkMessage::ToString() const
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
}
