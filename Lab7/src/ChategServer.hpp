#pragma once


class ChategServer
{
public:
	ChategServer();

	~ChategServer();


	void Start();


private:
	std::string _mailslotName;

	HANDLE _mailslot;
};
