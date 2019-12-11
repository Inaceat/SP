#pragma once


namespace TTT
{
	class NetworkMessage
	{
	public:
		enum class Type
		{
			ClientConnectionAsk,
			ClientMMAsk,
			ClientGameAction,
			ClientDisconnect,

			ServerMMResult,
			ServerGameState,
			ServerGameResult
		};


	public:
		NetworkMessage(Type type, std::string data) :
			_type(type),
			_data(data)
		{}


		Type GetType() const
		{
			return _type;
		}

		std::string GetData() const
		{
			return _data;
		}


		NetworkMessage(char* bytes, int bytesSize)
		{
			_type = static_cast<Type>(bytes[0]);

			_data = std::string(bytes + 1, bytesSize - 1);
		}

		char* GetMessageBytes(int* bytesSize) const
		{
			*bytesSize = 1 + static_cast<int>(_data.size()) + 1;//1 byte for type, size() for string, 1 for '\0'

			char* bytes = new char[*bytesSize];

			//Copy type
			bytes[0] = static_cast<char>(_type);

			//Copy data string
			strcpy_s(bytes + 1, *bytesSize - 1, _data.c_str());

			return bytes;
		}


	private:
		Type _type;
		std::string _data;
	};
}
