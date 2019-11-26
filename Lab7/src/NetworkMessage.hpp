#pragma once


namespace Chateg
{
	class NetworkMessage
	{
	public:
		enum class MessageType
		{
			Text,
			Register,
			Unregister
		};

	public:
		NetworkMessage(MessageType type, std::string sender, std::string text);

		
		static NetworkMessage* Create(char* messageBytes, int bytesSize);
	
		char* GetMessageBytes(int* bytesSize) const;
	

		MessageType Type() const;
		std::string Sender() const;
		std::string Text() const;


	private:
		NetworkMessage(char* messageBytes, int bytesSize);
	
	private:
		MessageType _type;

		std::string _sender;
		std::string _text;
	};
}