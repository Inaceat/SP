#pragma once

#include "SocketsTCP.hpp"
#include "SocketsUDP.hpp"
#include "NetworkMessage.hpp"



namespace TTT
{
	class Server
	{
	public:
		Server() :
			_registrationReceiver("42042")
		{}


		void Start()
		{
			_isWorking = true;
			_workerThread = std::thread([this]() { this->ProcessMessages(); });
		}

		void ProcessMessages()
		{
			int receiveTimeout = 100;

			sockaddr_in senderAddress;

			while (_isWorking)
			{
				NetworkMessage* message = _registrationReceiver.TryReceive(receiveTimeout, senderAddress);

				if (nullptr != message)
				{
					_tempSocket = ClientSocketTCP<NetworkMessage>("127.0.0.1:42042");

					Sleep(1000);
				}
			}
		}

	private:
		bool _isWorking;
		std::thread _workerThread;

		BroadcastReceiverSocketUDP<NetworkMessage> _registrationReceiver;

		ClientSocketTCP<NetworkMessage> _tempSocket;
	};
}
