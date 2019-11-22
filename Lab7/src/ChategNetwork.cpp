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


	void ChategNetwork::Start(const std::string& serverName, const std::string& clientName)
	{
		_clientName = clientName;


		DWORD nameSize;
		char nameBuffer[256];


		nameSize = 256;
		GetComputerNameEx(ComputerNameDnsDomain, nameBuffer, &nameSize);
		nameBuffer[nameSize] = 0;

		std::string domainName(nameBuffer);
		
		if("" == domainName)
			_domainMode = false;
		else
			_domainMode = true;


		nameSize = 256;
		GetComputerNameEx(ComputerNameDnsHostname, nameBuffer, &nameSize);
		nameBuffer[nameSize] = 0;

		_clientID = std::string(nameBuffer);



		std::string inPipeName = "\\\\.\\pipe\\" + clientName;
		
		std::string outMailslotName = "\\\\" + (_domainMode ? /*domainName*/std::string("*") : std::string(".")) + "\\mailslot\\" + serverName;


		_inPipe = new ServerSideNamedPipeConnection<ChategNetworkMessage>(inPipeName);

		_outMailslot = new ClientSideMailslotConnection<ChategNetworkMessage>(outMailslotName);
	}


	bool ChategNetwork::TryConnectToServer(int searchTimeout)
	{
		if (_outMailslot->TryConnect())
		{
			ChategNetworkMessage* registrationMessage = new ChategNetworkMessage(ChategNetworkMessage::MessageType::Register, (_domainMode ? _clientID : std::string(".")) + "$" + _clientName);

			_outMailslot->MessageSend(registrationMessage);

			delete registrationMessage;


			return _inPipe->WaitConnection(searchTimeout);
		}
		
		return false;
	}


	ChategNetworkMessage* ChategNetwork::MessageReceive(int timeout)
	{
		//TODO assuming message is never splitted, probably I'm wrong :)
		if(_inPipe->HasMessages())
			return _inPipe->MessageReceive();

		Sleep(timeout);//oh shi..

		if (_inPipe->HasMessages())
			return _inPipe->MessageReceive();
		
		return nullptr;
	}

	void ChategNetwork::MessageSend(ChategNetworkMessage* chategNetworkMessage)
	{
		_outMailslot->MessageSend(chategNetworkMessage);
	}

	
	void ChategNetwork::Stop()
	{
		MessageSend(new ChategNetworkMessage(ChategNetworkMessage::MessageType::Unregister, _domainMode ? _clientID : std::string(".") + "$" + _clientName));
	}
}
