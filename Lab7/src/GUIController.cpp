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


		CreateMessagePipe(_userCommandsIn, _userCommandsOut);
	}

	GUIController::~GUIController()
	{
		_isWorking = false;
		_workerThread.join();

		delete _inputController;
		delete _renderer;

		delete _userCommandsIn;
		delete _userCommandsOut;
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
		int timeout = 20;

		InputMessage* inputMessage;
		while (_isWorking)
		{
			inputMessage = _inputController->TryGetUserInput(timeout);
			if (nullptr != inputMessage)
			{
				//If exit command
				if (inputMessage->IsEscape())
				{
					_userCommandsIn->MessagePost(new UserMessage("", "stop"));//Meh..... so messy...
					continue;
				}

				_activeElementGuard.lock();
				auto currentActive = _activeElement;
				_activeElementGuard.unlock();

				switch (currentActive)
				{
					case ActiveGUIElement::Username: 
					{
						ProcessAsUsernameInput(inputMessage);
						break;
					}

					case ActiveGUIElement::Message:
					{
						ProcessAsMessageInput(inputMessage);
						break;
					}
				
					case ActiveGUIElement::None:
					{
						break;
					}
				}
			}
		}
	}

	void GUIController::ProcessAsUsernameInput(InputMessage* inputMessage)
	{
		//If letter, add to username
		if (inputMessage->IsLetterInput())
		{
			_username.push_back(inputMessage->Letter());

			_renderer->ShowUserName(_username);
		}
		//If backspace, remove letter
		else if (inputMessage->IsBackspace())
		{
			if (!_username.empty())
				_username.pop_back();

			_renderer->ShowUserName(_username);
		}
		//If enter, stop reading username
		else if (inputMessage->IsEnter())
		{
			//Only if username is not empty
			if (!_username.empty())
			{
				_activeElementGuard.lock();
				_activeElement = ActiveGUIElement::None;
				_activeElementGuard.unlock();
			}
		}
		//Else, do nothing

		delete inputMessage;
	}

	void GUIController::ProcessAsMessageInput(InputMessage* inputMessage)
	{
		//If letter, add to message
		if (inputMessage->IsLetterInput())
		{
			_message.push_back(inputMessage->Letter());

			_renderer->ShowCurrentInput(_message);
		}
		//If backspace, remove letter
		else if (inputMessage->IsBackspace())
		{
			if (!_message.empty())
			{
				_message.pop_back();
				
				_renderer->ShowCurrentInput(_message);
			}
		}
		//If enter, stop reading message, post it to pipe
		else if (inputMessage->IsEnter())
		{
			_userCommandsIn->MessagePost(new UserMessage(_username, _message));

			_message.erase();

			_renderer->ShowCurrentInput(_message);
		}
		//Else, do nothing

		delete inputMessage;
	}

	
	std::string GUIController::AskClientName()
	{
		_renderer->ShowUserNameInputPrompt();

		_activeElementGuard.lock();
		_activeElement = ActiveGUIElement::Username;
		_activeElementGuard.unlock();

		while (true)
		{
			_activeElementGuard.lock();
			auto currentActive = _activeElement;
			_activeElementGuard.unlock();

			if (ActiveGUIElement::Username != currentActive)
				break;
			//Do nothing, wait till username input ends
		}
		
		_renderer->HideUserNameInputPrompt();

		_activeElementGuard.lock();
		_activeElement = ActiveGUIElement::Message;
		_activeElementGuard.unlock();

		return _username;
	}

	UserMessage* GUIController::TryGetUserCommand(int timeout)
	{
		return _userCommandsOut->MessageGet(timeout);
	}

	void GUIController::ShowMessage(UserMessage* message)
	{
		_messages.push_back("[" + message->SenderName() + "]" + message->Text());

		delete message;

		_renderer->ShowMessages(_messages);
	}

	void GUIController::SetNetworkStatus(std::string newStatus)
	{
		_renderer->ShowNetworkStatus(newStatus);
	}
}
