#pragma once


namespace TTT
{
	template<typename TMessage>
	class MessagePipeInput;

	template<typename TMessage>
	class MessagePipeOutput;

	template<typename TMessage>
	void CreateMessagePipe(MessagePipeInput<TMessage>* & input, MessagePipeOutput<TMessage>* & output);


	template<typename TMessage>
	class MessagePipeOutput
	{
	private:
		MessagePipeOutput(const MessagePipeOutput<TMessage>&) = delete;
		MessagePipeOutput& operator=(const MessagePipeOutput<TMessage>&) = delete;

	public:
		~MessagePipeOutput()
		{
			CloseHandle(_outputPipeHandle);
		}


		TMessage* MessageGet(int timeout)
		{
			DWORD bytesRead;
			DWORD bytesAvailable;


			PeekNamedPipe(_outputPipeHandle, nullptr, 0, nullptr, &bytesAvailable, nullptr);

			if (bytesAvailable <  sizeof(int))
			{
				Sleep(timeout);

				PeekNamedPipe(_outputPipeHandle, nullptr, 0, nullptr, &bytesAvailable, nullptr);

				if (bytesAvailable <  sizeof(int))
					return nullptr;
			}


			int nextMessageSize;

			ReadFile(_outputPipeHandle, &nextMessageSize, sizeof(int), &bytesRead, nullptr);



			char* buffer = new char[nextMessageSize];

			ReadFile(_outputPipeHandle, buffer, nextMessageSize, &bytesRead, nullptr);


			TMessage* result = TMessage::Create(buffer, nextMessageSize);

			delete[] buffer;

			return result;
		}
	
	
	private:
		MessagePipeOutput(HANDLE outputPipe) :
			_outputPipeHandle(outputPipe)
		{}
	
		
		friend void CreateMessagePipe<TMessage>(MessagePipeInput<TMessage>* & input, MessagePipeOutput<TMessage>* & output);


	private:
		HANDLE _outputPipeHandle;
	
	};
}