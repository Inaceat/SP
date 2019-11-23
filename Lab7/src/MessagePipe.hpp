#pragma once


#include "MessagePipeInput.hpp"
#include "MessagePipeOutput.hpp"


#include <string>
using namespace Chateg;
class DummyMessage
{
public:
	DummyMessage(std::string text) : _text(text){}

	std::string ToString() const
	{
		return _text;
	}



	char* GetMessageBytes(int* bytesSize) const
	{
		*bytesSize = _text.size() + 1;

		char* bytes = new char[*bytesSize];

		strcpy_s(bytes, *bytesSize, _text.c_str());

		return bytes;
	}

	static DummyMessage* Create(char* bytes, int bytesSize)
	{
		return new DummyMessage(bytes, bytesSize);
	}

private:
	DummyMessage(char* bytes, int bytesSize) :
		_text(bytes, bytesSize)
	{}

	std::string _text;
};


namespace Chateg
{
	template<typename TMessage>
	void CreateMessagePipe(MessagePipeInput<TMessage>* & input, MessagePipeOutput<TMessage>* & output)
	{
		HANDLE outHandle;
		HANDLE inHandle;
	
		CreatePipe(&outHandle, &inHandle, nullptr, 0);
	
	
		input = new MessagePipeInput<TMessage>(inHandle);
	
		output = new MessagePipeOutput<TMessage>(outHandle);
	}
}
