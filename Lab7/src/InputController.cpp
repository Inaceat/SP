#include "stdafx.h"


#include "InputController.hpp"


namespace Chateg
{
	InputController::InputController():
		_isWorking(false)
	{
		CreateMessagePipe(_userInputIn, _userInputOut);
	}

	InputController::~InputController()
	{
		_isWorking = false;
		_workerThread.join();

		delete _userInputIn;
		delete _userInputOut;
	}


	void InputController::Start()
	{
		_isWorking = true;
		_workerThread = std::thread([this]() { this->Work(); });
	}


	InputMessage* InputController::TryGetUserInput(int timeout) const
	{
		return _userInputOut->MessageGet(timeout);
	}


	void InputController::Work()
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
}
