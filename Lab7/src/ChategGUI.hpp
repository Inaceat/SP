#pragma once
#include "SynchronizedQueue.hpp"


class ChategGUI
{
public:
	ChategGUI(SynchronizedQueue<std::string>* enteredMessages);
	
	~ChategGUI();
	
	
	void Init();

private:
	SynchronizedQueue<std::string>* _enteredMessages;
};
