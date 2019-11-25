#pragma once


#include "InputController.hpp"
#include "InputMessage.hpp"

#include "GUIRenderer.hpp"
#include "RendererCommand.hpp"

#include "UserMessage.hpp"


namespace Chateg
{
	class GUIController
	{
	public:
		GUIController();

		~GUIController();


		void Start();


		std::string AskClientName();

		UserMessage* TryGetUserMessage(int timeout);

		void ShowMessage(UserMessage* message);


	private:
		void Work();


	private:
		bool _isWorking;
		std::thread _workerThread;


		InputController* _inputController;

		GUIRenderer* _renderer;



		//std::mutex _messagesQueueGuard;
		//std::queue<UserMessage*> _messages;
	};
}