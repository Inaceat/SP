#pragma once


#pragma once
#include "UserCommand.hpp"


namespace TTT
{
	class GUIController
	{
	public:
		GUIController()
		{

		}


		void Start()
		{

		}


		std::string AskUserName()
		{
			return "DummyName";
		}

		UserCommand* TryGetUserCommand(int timeout)
		{
			return nullptr;
		}


		void ShowNetworkStatus(std::string newStatus)
		{
			std::cout << "Net: " << newStatus << std::endl;
		}

		

	private:

	};
}
