#pragma once


namespace Chateg
{
	//TODO make binary serialization using << >>?
	class ChategNetworkMessage
	{
	public:
		enum class MessageType
		{
			Text,
			Service
		};

	public:
		ChategNetworkMessage(MessageType type, std::string text);

		explicit ChategNetworkMessage(std::string text);


		static ChategNetworkMessage* Create(char* messageBytes, int bytesSize);
	
		char* GetMessageBytes(int* bytesSize) const;
	

		MessageType Type();
		std::string Data();

		std::string ToString() const;
	
	
	private:
		MessageType _type;
	
		std::string _text;
	};
}