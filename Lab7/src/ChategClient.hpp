#pragma once
#include "ChategServer.hpp"


class ChategClient
{
public:
	ChategClient();

	~ChategClient();


	void Start();


private:
	bool TryFindServer();


private:
	std::string _mailslotName = "\\\\.\\mailslot\\ChAtEg";

	ClientSideMailslotConnection<ChategMessage>* _mailslot;

	ChategServer* _server = nullptr;
};
