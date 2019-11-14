#pragma once
#include "ChategServer.hpp"
#include "NamedPipeConnection.hpp"
#include "ChategGUI.hpp"

class ChategClient
{
public:
	ChategClient();

	~ChategClient();


	void Start();


private:
	bool TryFindServer();

	static DWORD WINAPI MessageProcessingThread(LPVOID threadParam);

	void ProcessMessages();

private:
	std::string _pipeName = "\\\\.\\pipe\\_xXx_MeGa_BoSs_xXx_";
	std::string _mailslotName = "\\\\.\\mailslot\\ChAtEg";

	ServerSideNamedPipeConnection<ChategMessage>* _pipe;

	HANDLE _thread;

	ClientSideMailslotConnection<ChategMessage>* _mailslot;

	ChategServer* _server = nullptr;

	ChategGUI* _gui;
};
