#include "stdafx.h"


#include "InputMessage.hpp"


namespace TTT
{
	InputMessage::InputMessage(KEY_EVENT_RECORD keyInputInfo):
		_keyInputInfo(keyInputInfo)
	{
	}

	InputMessage::InputMessage(char* bytes, int bytesSize)
	{
		if (sizeof(KEY_EVENT_RECORD) < bytesSize)
			bytesSize = sizeof(KEY_EVENT_RECORD);

		memcpy_s(&_keyInputInfo, sizeof(KEY_EVENT_RECORD), bytes, bytesSize);
	}


	char* InputMessage::GetMessageBytes(int* messageLength)
	{
		*messageLength = sizeof(KEY_EVENT_RECORD);

		char* bytes = new char[*messageLength];

		memcpy_s(bytes, *messageLength, &_keyInputInfo, sizeof(KEY_EVENT_RECORD));

		return bytes;
	}

	InputMessage* InputMessage::Create(char* bytes, int bytesSize)
	{
		return new InputMessage(bytes, bytesSize);
	}


	bool InputMessage::IsDigitInput() const
	{
		return (0x30 <= _keyInputInfo.wVirtualKeyCode && _keyInputInfo.wVirtualKeyCode <= 0x39);
	}

	char InputMessage::Digit() const
	{
		return static_cast<char>(_keyInputInfo.wVirtualKeyCode);
	}


	bool InputMessage::IsLetterInput() const
	{
		return (0x41 <= _keyInputInfo.wVirtualKeyCode && _keyInputInfo.wVirtualKeyCode <= 0x5A);
	}

	char InputMessage::Letter() const
	{
		return _keyInputInfo.uChar.AsciiChar;
	}

	bool InputMessage::IsEnter() const
	{
		return VK_RETURN == _keyInputInfo.wVirtualKeyCode;
	}

	bool InputMessage::IsBackspace() const
	{
		return VK_BACK == _keyInputInfo.wVirtualKeyCode;
	}

	bool InputMessage::IsEscape() const
	{
		return VK_ESCAPE == _keyInputInfo.wVirtualKeyCode;
	}	
}
