#include "stdafx.h"

#include "Tasks.hpp"


DWORD Player(LPVOID param)
{
	while (true)
	{
		Beep(440, 500);
		Beep(440, 500);
		Beep(440, 500);
		Beep(349, 350);
		Beep(523, 150);
		Beep(440, 500);
		Beep(349, 350);
		Beep(523, 150);
		Beep(440, 1000);
		Beep(659, 500);
		Beep(659, 500);
		Beep(659, 500);
		Beep(698, 350);
		Beep(523, 150);
		Beep(415, 500);
		Beep(349, 350);
		Beep(523, 150);
		Beep(440, 1000);
	}
}


void Task2::Do()
{
	std::string command;

	DWORD playerThreadID;
	HANDLE playerHandle = NULL;

	while (true)
	{
		std::cout << "—клон€юсь перед ¬ашей волей..." << std::endl;
		std::cin >> command;

		if ("play" == command)
		{
			if (NULL == playerHandle)
				playerHandle = CreateThread(NULL, 0, Player, nullptr, 0, &playerThreadID);
			else
				ResumeThread(playerHandle);
		}
		else if ("pause" == command)
		{
			SuspendThread(playerHandle);
		}
		else if ("stop" == command)
		{
			TerminateThread(playerHandle, 0);
			return;
		}
		else
		{
			std::cout << "¬аша вол€ не€сна..." << std::endl;
		}
	}
}
