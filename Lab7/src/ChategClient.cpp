#include "stdafx.h"

#include "ChategClient.hpp"




ChategClient::ChategClient()
{
	_mailslotRead = CreateMailslot(_mailslotName.c_str(), 0, 0, nullptr);

	_mailslotWrite = CreateFile(_mailslotName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
}


ChategClient::~ChategClient()
{
	CloseHandle(_mailslotRead);

	CloseHandle(_mailslotWrite);
}



void ChategClient::Start()
{
	DWORD nextSize;

	char* readBuffer = new char[424];
	
	DWORD readBytes;
	DWORD writtenBytes;

	std::string userMessage;


	while (true)
	{
		GetMailslotInfo(_mailslotRead, nullptr, &nextSize, nullptr, nullptr);

		if (MAILSLOT_NO_MESSAGE != nextSize)
		{
			ReadFile(_mailslotRead, readBuffer, nextSize, &readBytes, nullptr);

			readBuffer[readBytes] = '\0';

			printf("%s", readBuffer);
		}

		std::cin >> userMessage;

		if("!q" == userMessage)
			break;

		WriteFile(_mailslotWrite, userMessage.c_str(), userMessage.size() * sizeof(char), &writtenBytes, nullptr);
	}

	delete[] readBuffer;
}
