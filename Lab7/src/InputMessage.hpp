#pragma once


namespace Chateg
{
	class InputMessage
	{
	public:
		explicit InputMessage(KEY_EVENT_RECORD keyInputInfo) :
			_keyInputInfo(keyInputInfo)
		{}


		char* GetMessageBytes(int* messageLength)
		{
			*messageLength = sizeof(KEY_EVENT_RECORD);

			char* bytes = new char[*messageLength];

			memcpy_s(bytes, *messageLength, &_keyInputInfo, sizeof(KEY_EVENT_RECORD));

			return bytes;
		}

		static InputMessage* Create(char* bytes, int bytesSize)
		{
			return new InputMessage(bytes, bytesSize);
		}


		bool IsLetterInput()
		{
			return (0x41 <= _keyInputInfo.wVirtualKeyCode && _keyInputInfo.wVirtualKeyCode <= 0x5A);
		}

		char Letter()
		{
			return _keyInputInfo.uChar.AsciiChar;
		}

		bool IsEnter()
		{
			return VK_RETURN == _keyInputInfo.wVirtualKeyCode;
		}

		bool IsBackspace()
		{
			return VK_BACK == _keyInputInfo.wVirtualKeyCode;
		}

		bool IsEscape()
		{
			return VK_ESCAPE == _keyInputInfo.wVirtualKeyCode;
		}


	private:
		InputMessage(char* bytes, int bytesSize)
		{
			if (sizeof(KEY_EVENT_RECORD) < bytesSize)
				bytesSize = sizeof(KEY_EVENT_RECORD);

			memcpy_s(&_keyInputInfo, sizeof(KEY_EVENT_RECORD), bytes, bytesSize);
		}


	private:
		KEY_EVENT_RECORD _keyInputInfo;
	};
}
