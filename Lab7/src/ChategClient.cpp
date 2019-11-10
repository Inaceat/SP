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

	_mailslot = new ClientSideMailslotConnection<ChategMessage>(_mailslotName);
}

ChategClient::~ChategClient()
{
	delete _mailslot;

	if (nullptr != _server)
		delete _server;
}


void ChategClient::Start()
{
	std::string userText;

	std::cin >> userText;

	ChategMessage* msg = new ChategMessage(userText);

	_mailslot->MessageSend(msg);

	delete msg;

	Sleep(4000);
}



bool ChategClient::TryFindServer()
{
	return false;
}
