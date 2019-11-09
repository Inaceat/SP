#pragma once



class ChategClient
{
public:
	ChategClient();

	~ChategClient();


	void Start();

	int Work();


private:
	std::string _mailslotName = "\\\\*\\mailslot\\ChAtEg";

	HANDLE _mailslot;
	
	HANDLE _messageProcessorThread;
};
