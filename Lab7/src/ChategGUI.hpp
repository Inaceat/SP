﻿#pragma once

#include "ChategGUICommand.hpp"


namespace Chateg
{
	class ChategGUI
	{
	public:
		ChategGUI();
		
		~ChategGUI();
		
		
		void Start();
		
		std::string AskClientName();
		
		ChategGUICommand* CommandReceive(int timeout);
		
		void ShowMessage(std::string messageText);


	private:
		void ConsoleWriter();

		void ConsoleReader();

	private:
		std::thread _guiOutputThread;
		std::thread _guiInputThread;

	};
}
