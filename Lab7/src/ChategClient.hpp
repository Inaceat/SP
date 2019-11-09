#pragma once



class ChategClient
{
public:
	ChategClient();

	~ChategClient();


	void Start();


private:
	std::string _mailslotName = "\\\\.\\mailslot\\ChAtEg";

	HANDLE _mailslotWrite;
	HANDLE _mailslotRead;
};
