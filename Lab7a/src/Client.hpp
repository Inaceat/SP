#pragma once


#include "NetworkController.hpp"


namespace TTT
{
	class Client
	{
	public:
		Client()
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

				//Connect to local server
				serverFound = _netController.TryFindServer(serverFindTimeout);

				//If connected to local, TODO
				if (serverFound)
				{
					
				}
				else//If failed to connect to local server, PANIC
					throw std::exception("Can't connect to local");
			}
			else
			{
				
			}




		}

	private:
		NetworkController _netController;
	};
}
