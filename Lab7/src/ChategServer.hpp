#pragma once


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
	HANDLE _mailslotRead;
};
