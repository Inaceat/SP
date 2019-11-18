#include "stdafx.h"


#include "ChategGUI.hpp"


namespace Chateg
{
	ChategGUI::ChategGUI()
	{

	}
	
	ChategGUI::~ChategGUI()
	{
		
	}
	
	
	void ChategGUI::Start()
	{
		_guiInputThread = std::thread( [this](){ this->ConsoleReader(); } );
		
		_guiOutputThread = std::thread( [this](){ this->ConsoleWriter(); } );
	}
	
	
	std::string ChategGUI::AskClientName()
	{
		return std::string("xXx_MegAboSs_xXx");
	}
	
	
	ChategGUICommand* ChategGUI::CommandReceive(int timeout)
	{


		return nullptr;//new ChategGUICommand(ChategGUICommand::CommandType::Message, "qweasd");
	}
	
	
	void ChategGUI::ShowMessage(std::string messageText)
	{
		std::cout << messageText << std::endl;
	}



	void ChategGUI::ConsoleWriter()
	{
		while (true)
		{
			//EnterCriticalSection(&queueCS);
			//
			//if (pressedKeys.empty())
			//{
			//	LeaveCriticalSection(&queueCS);
			//	continue;
			//}
			//
			//WORD key = pressedKeys.front();
			//pressedKeys.pop();
			//
			//LeaveCriticalSection(&queueCS);
			//
			//std::cout << (char)key;
			//
			//if (0x51 == key)
			//	break;
		}
	}

	void ChategGUI::ConsoleReader()
	{
		//HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
		//
		//INPUT_RECORD inputRecord;
		//
		//DWORD readEvents;


		while (true)
		{
			//ReadConsoleInput(inputHandle, &inputRecord, 1, &readEvents);
			//
			//
			//if (0 != readEvents && KEY_EVENT == inputRecord.EventType)
			//{
			//	if (0x41 <= inputRecord.Event.KeyEvent.wVirtualKeyCode && inputRecord.Event.KeyEvent.wVirtualKeyCode <= 0x5A)
			//	{
			//		if (TRUE == inputRecord.Event.KeyEvent.bKeyDown)
			//		{
			//		EnterCriticalSection(&queueCS);
			//			pressedKeys.push(inputRecord.Event.KeyEvent.wVirtualKeyCode);
			//		LeaveCriticalSection(&queueCS);
			//		}
			//
			//
			//	}
			//
			//	if (0x51 == inputRecord.Event.KeyEvent.wVirtualKeyCode)
			//		break;
			//}
		}
	}
}
