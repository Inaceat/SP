#include "stdafx.h"

#include "ChategClient.hpp"


ChategClient::ChategClient()
{
	bool serverFound = TryFindServer();

	if (!serverFound)
	{
		_server = new ChategServer(_mailslotName);
		_server->Start();
	}

	_mailslotWrite = CreateFile(_mailslotName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
}

ChategClient::~ChategClient()
{
	CloseHandle(_mailslotWrite);

	if (nullptr != _server)
		delete _server;
}


void ChategClient::Start()
{
	std::string userMessage;

	std::cin >> userMessage;

	DWORD bytesWritten;

	WriteFile(_mailslotWrite, userMessage.c_str(), sizeof(std::string::value_type) * userMessage.length(), &bytesWritten, nullptr);
}



bool ChategClient::TryFindServer()
{
	return false;
}
