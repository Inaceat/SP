#include "stdafx.h"


#include "NetworkMessage.hpp"


namespace Chateg
{
	NetworkMessage::NetworkMessage(MessageType type, std::string sender, std::string text) :
		_type(type),
		_sender(sender),
		_text(text)
	{}
	

	NetworkMessage::NetworkMessage(char* messageBytes, int bytesSize)
	{
		switch (messageBytes[0])
		{
			case 0:
				_type = MessageType::Text;
				break;

			case 1:
				_type = MessageType::Register;
				break;

			case 2:
				_type = MessageType::Unregister;
				break;

			default:
				throw "AAAAAAAAAAAAAAAAAAAAAAA";
		}

		_sender = std::string(messageBytes + 1);

		_text = std::string(messageBytes + 1 + _sender.size() + 1);
	}

	NetworkMessage* NetworkMessage::Create(char* messageBytes, int bytesSize)
	{
		return new NetworkMessage(messageBytes, bytesSize);
	}
	
	char* NetworkMessage::GetMessageBytes(int* bytesSize) const
	{
		*bytesSize = (1 + _sender.size() + 1 + _text.size() + 1);//+1 for MessageType, 2x +1 for '\0'
	
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

		//Sender
		memcpy_s(messageBytes + 1, _sender.size() + 1, _sender.c_str(), _sender.size() + 1);

		//Text
		memcpy_s(messageBytes + 1 + _sender.size() + 1, _text.size() + 1, _text.c_str(), _text.size() + 1);
	
		return messageBytes;
	}


	NetworkMessage::MessageType NetworkMessage::Type() const
	{
		return _type;
	}

	std::string NetworkMessage::Sender() const
	{
		return _sender;
	}

	std::string NetworkMessage::Text() const
	{
		return _text;
	}
}
