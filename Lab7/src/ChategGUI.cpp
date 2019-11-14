#include "stdafx.h"


#include "ChategGUI.hpp"


ChategGUI::ChategGUI()
{

}

ChategGUI::~ChategGUI()
{

}


void ChategGUI::Init()
{

}

/*
CRITICAL_SECTION queueCS;
std::queue<WORD> pressedKeys;


void ConsolePrinter()
{
	while (true)
	{
		EnterCriticalSection(&queueCS);
		
		if (pressedKeys.empty())
		{
			LeaveCriticalSection(&queueCS);
			continue;
		}

		WORD key = pressedKeys.front();
		pressedKeys.pop();

		LeaveCriticalSection(&queueCS);

		std::cout << (char)key;

		if (0x51 == key)
			break;
	}	
}





	InitializeCriticalSection(&queueCS);


	std::thread conOut(ConsolePrinter);



	HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
	
	INPUT_RECORD inputRecord;

	DWORD readEvents;
	
	while (true)
	{
		ReadConsoleInput(inputHandle, &inputRecord, 1, &readEvents);

		if (0 != readEvents && KEY_EVENT == inputRecord.EventType)
		{
			if (0x41 <= inputRecord.Event.KeyEvent.wVirtualKeyCode && inputRecord.Event.KeyEvent.wVirtualKeyCode <= 0x5A)
			{
				if (TRUE == inputRecord.Event.KeyEvent.bKeyDown)
				{
				EnterCriticalSection(&queueCS);
					pressedKeys.push(inputRecord.Event.KeyEvent.wVirtualKeyCode);
				LeaveCriticalSection(&queueCS);
				}


			}

			if (0x51 == inputRecord.Event.KeyEvent.wVirtualKeyCode)
				break;
		}
	}


	conOut.join();

 */
