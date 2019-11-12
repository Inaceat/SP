#pragma once

#include "ChategMessage.hpp"


#include "MailslotConnection.hpp"
#include "NamedPipeConnection.hpp"


class ChategServer
{
public:
	ChategServer(std::string mailslotName);

	~ChategServer();


	void Start();

private:
	static DWORD WINAPI MessageProcessingThread(LPVOID threadParam);
	
	void ProcessMessages();

private:
	HANDLE _thread;
	ServerSideMailslotConnection<ChategMessage>* _mailslot;

	std::vector<ClientSideNamedPipeConnection<ChategMessage>*> _clients;
};
