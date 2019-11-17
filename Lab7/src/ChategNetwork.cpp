#include "stdafx.h"


#include "ChategNetwork.hpp"


namespace Chateg
{
	ChategNetwork::ChategNetwork() :
		_outMailslot(nullptr),
		_inPipe(nullptr)
	{}

	ChategNetwork::~ChategNetwork()
	{
		delete _outMailslot;

		delete _inPipe;
	}


	void ChategNetwork::Start(const std::string& serverName, const std::string& clientID)
	{
		_clientID = clientID;


		std::string inPipeName = "\\\\.\\pipe\\" + clientID;
		
		std::string outMailslotName = "\\\\.\\mailslot\\" + serverName;


		_inPipe = new ServerSideNamedPipeConnection<ChategNetworkMessage>(inPipeName);

		_outMailslot = new ClientSideMailslotConnection<ChategNetworkMessage>(outMailslotName);
	}


	bool ChategNetwork::TryConnectToServer(int searchTimeout)
	{
		ChategNetworkMessage* registrationMessage = new ChategNetworkMessage(ChategNetworkMessage::MessageType::Service, _clientID);

		_outMailslot->MessageSend(registrationMessage);

		delete registrationMessage;


		return _inPipe->WaitConnection(searchTimeout);	
	}


	ChategNetworkMessage* ChategNetwork::MessageReceive(int timeout)
	{
		//TODO assuming message is never splitted, probably I'm wrong :)



		return _inPipe->MessageReceive();
	}

	void ChategNetwork::MessageSend(ChategNetworkMessage* chategNetworkMessage)
	{
		_outMailslot->MessageSend(chategNetworkMessage);
	}
}
