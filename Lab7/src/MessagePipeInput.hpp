#pragma once


namespace Chateg
{
	template<typename TMessage>
	class MessagePipeInput;

	template<typename TMessage>
	class MessagePipeOutput;

	template<typename TMessage>
	void CreateMessagePipe(MessagePipeInput<TMessage>* & input, MessagePipeOutput<TMessage>* & output);



	template<typename TMessage>
	class MessagePipeInput
	{
	private:
		MessagePipeInput(const MessagePipeInput&) = delete;
		MessagePipeInput& operator=(const MessagePipeInput&) = delete;

	public:
		~MessagePipeInput()
		{
			CloseHandle(_inputPipeHandle);
		}


		void MessagePost(TMessage* message)
		{
			int messageLength;
			char* messageBytes = message->GetMessageBytes(&messageLength);

			DWORD bytesWritten;

			WriteFile(_inputPipeHandle, &messageLength, sizeof(int), &bytesWritten, nullptr);//Message size
			WriteFile(_inputPipeHandle, messageBytes, messageLength, &bytesWritten, nullptr);//Message bytes

			delete[] messageBytes;

			delete message;
		}
	
	
	private:
		MessagePipeInput(HANDLE inputPipe) :
			_inputPipeHandle(inputPipe)
		{}
	
	
		friend void CreateMessagePipe<TMessage>(MessagePipeInput<TMessage>* & input, MessagePipeOutput<TMessage>* & output);

	
	private:
		HANDLE _inputPipeHandle;
	
	};
}