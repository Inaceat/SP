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
		std::string outMailslotName = std::string("\\\\") + (_domainMode ? "*" : ".") + "\\mailslot\\" + serverName;

		_outMailslot = new ClientSideMailslotConnection<NetworkMessage>(outMailslotName);
	}


	bool NetworkController::TryConnectToServerAs(std::string userName, int searchTimeout)
	{
		std::cout << "Trying to connect as \"" << userName << "\" from \"" << _clientID << "\"" << std::endl;
		if (_outMailslot->TryConnect())
		{
			std::string inPipeName = "\\\\.\\pipe\\" + userName;

			_inPipe = new ServerSideNamedPipeConnection<NetworkMessage>(inPipeName);


			NetworkMessage* registrationMessage = new NetworkMessage(NetworkMessage::MessageType::Register, userName, _clientID);
		
			MessageSend(registrationMessage);
		
			delete registrationMessage;


			bool connectionSuccessful = _inPipe->WaitConnection(searchTimeout);

			if (connectionSuccessful)
			{
				_connectedAsName = userName;
				return true;
			}

			delete _inPipe;
			_inPipe = nullptr;
			_outMailslot->Disconnect();
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
		std::cout << "Sending \"" << chategNetworkMessage->Text() << "\" from \"" << chategNetworkMessage->Sender() << "\"" << std::endl;
		_outMailslot->MessageSend(chategNetworkMessage);
	}

	
	void NetworkController::Disconnect()
	{
		MessageSend(new NetworkMessage(NetworkMessage::MessageType::Unregister, _connectedAsName, _clientID));
	}
}
