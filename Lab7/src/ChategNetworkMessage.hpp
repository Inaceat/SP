#pragma once


namespace Chateg
{
	class ChategNetworkMessage
	{
	public:
		enum class MessageType
		{
			Text,
			Register,
			Unregister
		};

	public:
		ChategNetworkMessage(MessageType type, std::string text);

		explicit ChategNetworkMessage(std::string text);


		static ChategNetworkMessage* Create(char* messageBytes, int bytesSize);
	
		char* GetMessageBytes(int* bytesSize) const;
	

		MessageType Type() const;
		std::string Data() const;

		std::string ToString() const;
	
	
	private:
		MessageType _type;
	
		std::string _text;
	};
}