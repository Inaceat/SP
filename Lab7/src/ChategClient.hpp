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

	HANDLE _mailslotWrite;

	ChategServer* _server = nullptr;
};
