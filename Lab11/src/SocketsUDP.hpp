#pragma once


#include "SocketsUtils.hpp"


template<typename TMessage>
class BroadcastSenderSocketUDP
{
	//No copy allowed!
	BroadcastSenderSocketUDP(BroadcastSenderSocketUDP& other) = delete;
	void operator=(BroadcastSenderSocketUDP& other) = delete;

public:
	//Addresses should be formatted as "IP1.IP2.IP3.IP4:PORT"
	explicit BroadcastSenderSocketUDP(std::string localAddress, std::string targetAddress) :
		_socketPtr(nullptr)
	{
		sockaddr_in local;
		ZeroMemory(&local, sizeof(local));

		local.sin_family = AF_INET;
		local.sin_port = htons(atoi(localAddress.substr(localAddress.find(":") + 1, std::string::npos).c_str()));
		inet_pton(AF_INET, localAddress.substr(0, localAddress.find(":")).c_str(), &(local.sin_addr));


		//Target address
		ZeroMemory(&_targetAddress, sizeof(_targetAddress));
		
		_targetAddress.sin_family = AF_INET;
		_targetAddress.sin_port = htons(atoi(targetAddress.substr(targetAddress.find(":") + 1, std::string::npos).c_str()));
		inet_pton(AF_INET, targetAddress.substr(0, targetAddress.find(":")).c_str(), &(_targetAddress.sin_addr));


		//Socket
		SOCKET newSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		//Enable broadcast
		BOOL opt = TRUE;
		setsockopt(newSocket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&opt), sizeof(opt));


		bind(newSocket, reinterpret_cast<sockaddr*>(&local), sizeof(local));

		_socketPtr.reset(new SOCKET(newSocket));
	}

	~BroadcastSenderSocketUDP()
	{
		if (nullptr != _socketPtr)
			shutdown(*_socketPtr, SD_BOTH);
	}


	void Send(TMessage message)
	{
		//Convert message to bytes
		int messageLength;
		char* messageBytes = message.GetMessageBytes(&messageLength);

		//Allocate buffer
		int bytesToSend = sizeof(messageLength) + messageLength;
		char* bufferToSend = new char[bytesToSend];

		//Copy message size to buffer
		memcpy_s(bufferToSend, bytesToSend, &messageLength, sizeof(messageLength));

		//Copy message to buffer
		memcpy_s(bufferToSend + sizeof(messageLength), messageLength, messageBytes, messageLength);

		delete[] messageBytes;


		//Send
		sendto(*_socketPtr, bufferToSend, bytesToSend, 0, reinterpret_cast<sockaddr*>(&_targetAddress), sizeof(_targetAddress));


		delete[] bufferToSend;
	}

private:
	std::unique_ptr<SOCKET, SocketDeleter> _socketPtr;

	sockaddr_in _targetAddress;
};


template<typename TMessage>
class BroadcastReceiverSocketUDP
{
	//No copy allowed!
	BroadcastReceiverSocketUDP(BroadcastReceiverSocketUDP& other) = delete;
	void operator=(BroadcastReceiverSocketUDP& other) = delete;

public:
	//Should be valid port
	explicit BroadcastReceiverSocketUDP(std::string port) :
		_socketPtr(nullptr)
	{
		//Create new socket
		SOCKET newSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		//Enable broadcasting
		BOOL opt = TRUE;
		int res = setsockopt(newSocket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&opt), sizeof(opt));

		//Set non-blocking mode
		u_long mode = 1;
		ioctlsocket(newSocket, FIONBIO, &mode);

		//Set address automatically
		sockaddr_in local;
		ZeroMemory(&local, sizeof(local));

		local.sin_family = AF_INET;
		local.sin_port = htons(atoi(port.c_str()));
		local.sin_addr.S_un.S_addr = INADDR_ANY;

		bind(newSocket, reinterpret_cast<sockaddr*>(&local), sizeof(local));


		_socketPtr.reset(new SOCKET(newSocket));
	}

	~BroadcastReceiverSocketUDP()
	{
		if (nullptr != _socketPtr)
			shutdown(*_socketPtr, SD_BOTH);
	}


	TMessage* TryReceive(int timeout, sockaddr_in& senderAddressInfo)
	{
		//Prepare for waiting
		timeval time;
		time.tv_sec = timeout / 1000;			//seconds
		time.tv_usec = (timeout % 1000) * 1000;	//microseconds, timeout is 'ms', so to get 'us' we multiply by 1000

		fd_set set;
		FD_ZERO(&set);
		FD_SET(*_socketPtr, &set);


		//Wait till data available
		int readySocketsCount = select(9001, &set, nullptr, nullptr, &time);//First param is ignored

		//If error occured, PANIC
		if(SOCKET_ERROR == readySocketsCount)
		{
			shutdown(*_socketPtr, SD_BOTH);
			_socketPtr.reset(nullptr);

			return nullptr;
		}

		//If socket has no data after timeout, return nothing
		if (0 == readySocketsCount)
		{
			return nullptr;
		}


		//Prepare sender address buffer
		sockaddr_in senderAddress;
		int senderAddressSize = sizeof(senderAddress);
		ZeroMemory(&senderAddress, sizeof(senderAddress));

		//Buffer size
		unsigned int receiveBufferSize;
		int bufferSizeSize = sizeof(receiveBufferSize);

		getsockopt(*_socketPtr, SOL_SOCKET, SO_MAX_MSG_SIZE, reinterpret_cast<char*>(&receiveBufferSize), &bufferSizeSize);

		//Buffer itself
		char* receiveBuffer = new char[receiveBufferSize];

		//Read
		int bytesRead = recvfrom(*_socketPtr, receiveBuffer, receiveBufferSize, 0, reinterpret_cast<sockaddr*>(&senderAddress), &senderAddressSize);


		//Get message length
		int messageLength;

		//If not enough data for length received
		if (bytesRead < sizeof(messageLength))
		{
			delete[] receiveBuffer;
			return nullptr;
		}

		memcpy_s(&messageLength, sizeof(messageLength), receiveBuffer, sizeof(messageLength));

		//If not enough data for full message read
		if (bytesRead - sizeof(messageLength) < messageLength)
		{
			delete[] receiveBuffer;
			return nullptr;
		}

		TMessage* resultMessage = new TMessage(receiveBuffer + sizeof(messageLength), messageLength);

		delete[] receiveBuffer;

		//Return data
		senderAddressInfo = senderAddress;

		return resultMessage;
	}


private:
	std::unique_ptr<SOCKET, SocketDeleter> _socketPtr;
};
