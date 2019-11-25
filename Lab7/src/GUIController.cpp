#include "stdafx.h"


#include "GUIController.hpp"
#include "InputMessage.hpp"


namespace Chateg
{
	GUIController::GUIController() :
		_isWorking(false)
	{
		_inputController = new InputController();

		_renderer = new GUIRenderer();
	}

	GUIController::~GUIController()
	{
		_isWorking = false;
		_workerThread.join();

		delete _inputController;
		delete _renderer;
	}


	void GUIController::Start()
	{
		_isWorking = true;
		_workerThread = std::thread( [this]() { this->Work(); } );


		_inputController->Start();
		_renderer->Start();
	}


	void GUIController::Work()
	{
		while (_isWorking)
		{

		}
	}


	
	std::string GUIController::AskClientName()
	{
		_renderer->ShowUserNameInputPrompt();


		std::string userName = "";

		InputMessage* inputMessage;
		while(true)
		{
			inputMessage = _inputController->TryGetUserInput(20);
			
			if (nullptr == inputMessage)
				continue;

			//If letter, add to username
			if (inputMessage->IsLetterInput())
			{
				userName.push_back(inputMessage->Letter());
				delete inputMessage;

				_renderer->ShowUserName(userName);
			}
			//If backspace, remove letter
			else if (inputMessage->IsBackspace())
			{
				if (!userName.empty())
					userName.pop_back();

				delete inputMessage;

				_renderer->ShowUserName(userName);
			}
			//If enter, stop reading username
			else if (inputMessage->IsEnter())
			{
				delete inputMessage;
				break;
			}
			//Else, do nothing
			else
			{
				delete inputMessage;
			}
		}


		_renderer->HideUserNameInputPrompt();


		return userName;
	}

	UserMessage* GUIController::TryGetUserMessage(int timeout)
	{
		return nullptr;
	}

	void GUIController::ShowMessage(UserMessage* message)
	{

	}
}
