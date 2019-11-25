#pragma once


#include "InputMessage.hpp"
#include "MessagePipe.hpp"


namespace Chateg
{
	class InputController
	{
	public:
		InputController() :
			_isWorking(false)
		{
			CreateMessagePipe(_userInputIn, _userInputOut);
		}

		~InputController()
		{
			_isWorking = false;
			_workerThread.join();

			delete _userInputIn;
			delete _userInputOut;
		}


		void Start()
		{
			_isWorking = true;
			_workerThread = std::thread( [this]() { this->Work(); } );
		}


		InputMessage* TryGetUserInput(int timeout) const
		{
			return _userInputOut->MessageGet(timeout);
		}


	private:
		void Work()
		{
			DWORD timeout = 20;

			HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
			INPUT_RECORD inputRecord;
			DWORD readEvents;

			while (_isWorking)
			{
				if (WAIT_OBJECT_0 != WaitForSingleObject(inputHandle, timeout))
					continue;

				ReadConsoleInput(inputHandle, &inputRecord, 1, &readEvents);

				if (0 != readEvents && KEY_EVENT == inputRecord.EventType && inputRecord.Event.KeyEvent.bKeyDown)
				{
					_userInputIn->MessagePost(new InputMessage(inputRecord.Event.KeyEvent));
				}
			}
		}


	private:
		bool _isWorking;
		std::thread _workerThread;

		MessagePipeInput<InputMessage>* _userInputIn;
		MessagePipeOutput<InputMessage>* _userInputOut;
	};
}
