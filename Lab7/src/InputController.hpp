#pragma once


#include "InputMessage.hpp"
#include "MessagePipe.hpp"


namespace Chateg
{
	class InputController
	{
	public:
		InputController();

		~InputController();


		void Start();


		InputMessage* TryGetUserInput(int timeout) const;


	private:
		void Work();


	private:
		bool _isWorking;
		std::thread _workerThread;

		MessagePipeInput<InputMessage>* _userInputIn;
		MessagePipeOutput<InputMessage>* _userInputOut;
	};
}
