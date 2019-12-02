#pragma once


namespace Chateg
{
	class UserMessage
	{
	public:
		UserMessage(std::string senderName, std::string text);


		std::string SenderName() const;

		std::string Text() const;


		char* GetMessageBytes(int* messageLength);

		static UserMessage* Create(char* bytes, int bytesSize);


	private:
		UserMessage(char* bytes, int bytesSize);

	private:
		std::string _senderName;
		std::string _text;
	};
}
