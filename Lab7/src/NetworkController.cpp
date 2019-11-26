#include "stdafx.h"


#include "NetworkController.hpp"


namespace Chateg
{
	NetworkController::NetworkController() :
		_outMailslot(nullptr),
		_inPipe(nullptr)
	{
		DWORD nameSize;
		char nameBuffer[256];


		nameSize = 256;
		GetComputerNameEx(ComputerNameDnsDomain, nameBuffer, &nameSize);
		nameBuffer[nameSize] = 0;

		std::string domainName(nameBuffer);

		if ("" == domainName)
			_domainMode = false;
		else
			_domainMode = true;


		nameSize = 256;
		GetComputerNameEx(ComputerNameDnsHostname, nameBuffer, &nameSize);
		nameBuffer[nameSize] = 0;

		_clientID = std::string(nameBuffer);
	}

	NetworkController::~NetworkController()
	{
		delete _outMailslot;

		delete _inPipe;
	}


	void NetworkController::Start(const std::string& serverName)
	{
		std::string inPipeName = "\\\\.\\pipe\\" + _clientID;
		
		std::string outMailslotName = std::string("\\\\") + (_domainMode ? "*" : ".") + "\\mailslot\\" + serverName;


		_inPipe = new ServerSideNamedPipeConnection<NetworkMessage>(inPipeName);

		_outMailslot = new ClientSideMailslotConnection<NetworkMessage>(outMailslotName);
	}


	bool NetworkController::TryConnectToServerAs(std::string userName, int searchTimeout)
	{
		if (_outMailslot->TryConnect())
		{
			NetworkMessage* registrationMessage = new NetworkMessage(NetworkMessage::MessageType::Register, userName, _clientID);
		
			_outMailslot->MessageSend(registrationMessage);
		
			delete registrationMessage;
		
			return _inPipe->WaitConnection(searchTimeout);
		}
		
		return false;
	}


	NetworkMessage* NetworkController::MessageReceive(int timeout)
	{
		//TODO assuming message is never splitted, probably I'm wrong :)
		if(_inPipe->HasMessages())
			return _inPipe->MessageReceive();

		Sleep(timeout);//oh shi..

		if (_inPipe->HasMessages())
			return _inPipe->MessageReceive();
		
		return nullptr;
	}

	void NetworkController::MessageSend(NetworkMessage* chategNetworkMessage)
	{
		_outMailslot->MessageSend(chategNetworkMessage);
	}

	
	void NetworkController::Disconnect()
	{
		//MessageSend(new NetworkMessage(NetworkMessage::MessageType::Unregister, _domainMode ? _clientID : std::string(".") + "$" + _clientName));
	}
}
