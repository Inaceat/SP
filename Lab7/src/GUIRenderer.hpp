#pragma once


namespace Chateg
{
	class GUIRenderer
	{
	public:
		GUIRenderer();

		~GUIRenderer();


		void Start();


		void ShowUserNameInputPrompt();

		void HideUserNameInputPrompt();


		void ShowCurrentInput(std::string currentInputString);

		void ShowMessages(std::vector<std::string> messages);

		void ShowUserName(std::string userName);

		void ShowNetworkStatus(std::string newStatus);


	private:
		HANDLE _oldScreenBuffer;
		
		HANDLE _mainScreenBuffer;
		CONSOLE_SCREEN_BUFFER_INFO _mainScreenInfo;

		std::string _networkStatusLabel = "Network status: ";
	};
}
