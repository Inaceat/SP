#pragma once


namespace Chateg
{
	class UserMessage
	{
	public:
		UserMessage(std::string senderName, std::string text) :
			_senderName(senderName),
			_text(text)
		{}


		std::string SenderName() const
		{
			return _senderName;
		}

		std::string Text() const
		{
			return _text;
		}


		char* GetMessageBytes(int* messageLength)
		{
			*messageLength = _senderName.size() + 1 + _text.size() + 1;

			char* bytes = new char[*messageLength];


			memcpy_s(bytes, _senderName.size() + 1, _senderName.c_str(), _senderName.size() + 1);

			memcpy_s(bytes + _senderName.size() + 1, _text.size() + 1, _text.c_str(), _text.size() + 1);

			
			return bytes;
		}

		static UserMessage* Create(char* bytes, int bytesSize)
		{
			return new UserMessage(bytes, bytesSize);
		}


	private:
		UserMessage(char* bytes, int bytesSize)
		{
			_senderName = std::string(bytes);

			_text = std::string(bytes + _senderName.size() + 1);
		}

	private:
		std::string _senderName;
		std::string _text;
	};
}
