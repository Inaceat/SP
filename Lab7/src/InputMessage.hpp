#pragma once


namespace Chateg
{
	class InputMessage
	{
	public:
		explicit InputMessage(KEY_EVENT_RECORD keyInputInfo);


		char* GetMessageBytes(int* messageLength);

		static InputMessage* Create(char* bytes, int bytesSize);


		bool IsLetterInput() const;

		char Letter() const;

		bool IsEnter() const;

		bool IsBackspace() const;

		bool IsEscape() const;


	private:
		InputMessage(char* bytes, int bytesSize);


	private:
		KEY_EVENT_RECORD _keyInputInfo;
	};
}
