#include "stdafx.h"

#include "Tasks.hpp"


#include "ChategClient.hpp"


void Task3::Do()
{
	auto client = new Chateg::ChategClient();
	
	client->Start();


	delete client;
}
