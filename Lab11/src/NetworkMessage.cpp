#include "stdafx.h"


#include "NetworkMessage.hpp"


namespace TTT
{
	NetworkMessage::NetworkMessage(Type type, std::string data):
		_type(type),
		_data(data)
	{
	}


	NetworkMessage::Type NetworkMessage::GetType() const
	{
		return _type;
	}

	std::string NetworkMessage::GetData() const
	{
		return _data;
	}


	NetworkMessage::NetworkMessage(char* bytes, int bytesSize)
	{
		_type = static_cast<Type>(bytes[0]);

		_data = std::string(bytes + 1, bytesSize - 2);
	}

	char* NetworkMessage::GetMessageBytes(int* bytesSize) const
	{
		*bytesSize = 1 + static_cast<int>(_data.size()) + 1; //1 byte for type, size() for string, 1 for '\0'

		char* bytes = new char[*bytesSize];

		//Copy type
		bytes[0] = static_cast<char>(_type);

		//Copy data string
		memcpy_s(bytes + 1, *bytesSize - 1, _data.c_str(), _data.size() + 1);

		return bytes;
	}
}
