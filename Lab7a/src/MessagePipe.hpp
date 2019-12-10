#pragma once


#include "MessagePipeInput.hpp"
#include "MessagePipeOutput.hpp"


namespace TTT
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
