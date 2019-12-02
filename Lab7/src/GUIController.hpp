#pragma once


#include "InputController.hpp"
#include "InputMessage.hpp"

#include "GUIRenderer.hpp"

#include "UserMessage.hpp"


namespace Chateg
{
	class GUIController
	{
	private:
		enum class ActiveGUIElement
		{
			None,
			Username,
			Message
		};


	public:
		GUIController();

		~GUIController();


		void Start();


		void SetNetworkStatus(std::string newStatus);

		std::string AskClientName();

		UserMessage* TryGetUserCommand(int timeout);

		void ShowMessage(UserMessage* message);


	private:
		void Work();

		void ProcessAsUsernameInput(InputMessage* inputMessage);
		void ProcessAsMessageInput(InputMessage* inputMessage);

	private:
		bool _isWorking;
		std::thread _workerThread;


		InputController* _inputController;
		GUIRenderer* _renderer;

		std::mutex _activeElementGuard;
		ActiveGUIElement _activeElement;

		std::vector<std::string> _messages;
		std::string _message;
		std::string _username;

		MessagePipeInput<UserMessage>* _userCommandsIn;
		MessagePipeOutput<UserMessage>* _userCommandsOut;
	};
}