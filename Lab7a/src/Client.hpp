#pragma once


#include "NetworkController.hpp"
#include "Server.hpp"


namespace TTT
{
	class Client
	{
	public:
		Client() :
			_localServer(nullptr)
		{
			
		}


		void Start()
		{
			//Try find server
			int serverFindTimeout = 1000;

			bool serverFound = _netController.TryFindServer(serverFindTimeout);

			if (!serverFound)
			{
				//Create own server
				_localServer = new Server();
				_localServer->Start();

				//Connect to local server
				serverFound = _netController.TryFindServer(serverFindTimeout);

				//If connected to local, TODO
				if (serverFound)
				{
					std::cout << "connected to local" << std::endl;
				}
				else//If failed to connect to local server, PANIC
					throw std::exception("Can't connect to local");
			}
			else
			{
				std::cout << "connected to remote" << std::endl;
			}




		}

	private:
		NetworkController _netController;

		Server* _localServer;
	};
}
