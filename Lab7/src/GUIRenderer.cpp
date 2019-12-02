#include "stdafx.h"


#include "GUIRenderer.hpp"


//Converting std::string::size_type to DWORD
//causes too many warnings for this simple lab :)
#pragma warning(disable: 4267)


namespace Chateg
{
	GUIRenderer::GUIRenderer():
		_oldScreenBuffer(nullptr),
		_mainScreenBuffer(nullptr)
	{
	}

	GUIRenderer::~GUIRenderer()
	{
		SetConsoleActiveScreenBuffer(_oldScreenBuffer);

		CloseHandle(_mainScreenBuffer);
	}


	void GUIRenderer::Start()
	{
		//Buffers
		_oldScreenBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
		_mainScreenBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr,
		                                              CONSOLE_TEXTMODE_BUFFER, nullptr);

		//Initial "image"
		GetConsoleScreenBufferInfo(_mainScreenBuffer, &_mainScreenInfo);


		std::string line(_mainScreenInfo.dwSize.X, '-');

		DWORD written;

		SetConsoleCursorPosition(_mainScreenBuffer, {0, 0});
		WriteConsole(_mainScreenBuffer, _networkStatusLabel.c_str(), _networkStatusLabel.size(), &written, nullptr);

		SetConsoleCursorPosition(_mainScreenBuffer, {0, 1});
		WriteConsole(_mainScreenBuffer, line.c_str(), line.size(), &written, nullptr);

		SetConsoleCursorPosition(_mainScreenBuffer, {0, _mainScreenInfo.dwSize.Y - 5});
		WriteConsole(_mainScreenBuffer, line.c_str(), line.size(), &written, nullptr);

		SetConsoleCursorPosition(_mainScreenBuffer, {0, _mainScreenInfo.dwSize.Y - 3});
		WriteConsole(_mainScreenBuffer, line.c_str(), line.size(), &written, nullptr);


		SetConsoleActiveScreenBuffer(_mainScreenBuffer);
	}


	void GUIRenderer::ShowUserNameInputPrompt()
	{
		std::string prompt("Please, enter username");
		//Sorry, all "images" are ugly.
		//"***...***"
		std::string promptFirstLine(_mainScreenInfo.dwSize.X / 2, '*');
		//"*   ...   prompt   ...   *"
		std::string promptSecondLine = "*" + std::
			string((((_mainScreenInfo.dwSize.X / 2) - prompt.length()) / 2) - 1, ' ') + prompt + std::string(
				(((_mainScreenInfo.dwSize.X / 2) - prompt.length()) / 2) - 1, ' ') + " *";
		//"***...***"
		std::string promptThirdLine(_mainScreenInfo.dwSize.X / 2, '*');


		DWORD written;

		SetConsoleCursorPosition(_mainScreenBuffer, {
			(_mainScreenInfo.dwSize.X - (SHORT)promptFirstLine.length()) / 2, _mainScreenInfo.dwSize.Y / 2 - 3
		});
		WriteConsole(_mainScreenBuffer, promptFirstLine.c_str(), promptFirstLine.size(), &written, nullptr);

		SetConsoleCursorPosition(_mainScreenBuffer, {
			(_mainScreenInfo.dwSize.X - (SHORT)promptSecondLine.length()) / 2, _mainScreenInfo.dwSize.Y / 2 - 2
		});
		WriteConsole(_mainScreenBuffer, promptSecondLine.c_str(), promptSecondLine.size(), &written, nullptr);

		SetConsoleCursorPosition(_mainScreenBuffer, {
			(_mainScreenInfo.dwSize.X - (SHORT)promptThirdLine.length()) / 2, _mainScreenInfo.dwSize.Y / 2 - 1
		});
		WriteConsole(_mainScreenBuffer, promptThirdLine.c_str(), promptThirdLine.size(), &written, nullptr);
	}

	void GUIRenderer::HideUserNameInputPrompt()
	{
		static std::string emptyLine(_mainScreenInfo.dwSize.X, ' ');

		DWORD written;


		SetConsoleCursorPosition(_mainScreenBuffer, {0, _mainScreenInfo.dwSize.Y / 2 - 3});
		WriteConsole(_mainScreenBuffer, emptyLine.c_str(), emptyLine.size(), &written, nullptr);

		SetConsoleCursorPosition(_mainScreenBuffer, {0, _mainScreenInfo.dwSize.Y / 2 - 2});
		WriteConsole(_mainScreenBuffer, emptyLine.c_str(), emptyLine.size(), &written, nullptr);

		SetConsoleCursorPosition(_mainScreenBuffer, {0, _mainScreenInfo.dwSize.Y / 2 - 1});
		WriteConsole(_mainScreenBuffer, emptyLine.c_str(), emptyLine.size(), &written, nullptr);
	}


	void GUIRenderer::ShowCurrentInput(std::string currentInputString)
	{
		static std::string emptyInput(_mainScreenInfo.dwSize.X, ' ');

		DWORD written;

		SetConsoleCursorPosition(_mainScreenBuffer, {0, _mainScreenInfo.dwSize.Y - 4});
		WriteConsole(_mainScreenBuffer, emptyInput.c_str(), emptyInput.size(), &written, nullptr);

		SetConsoleCursorPosition(_mainScreenBuffer, {0, _mainScreenInfo.dwSize.Y - 4});
		WriteConsole(_mainScreenBuffer, currentInputString.c_str(), currentInputString.size(), &written, nullptr);
	}

	void GUIRenderer::ShowMessages(std::vector<std::string> messages)
	{
		static std::string emptyInput(_mainScreenInfo.dwSize.X, ' ');

		DWORD written;

		SHORT currentLine = 2;
		auto currentMessage = messages.begin();
		//While available lines and messages left
		for (; _mainScreenInfo.dwSize.Y - 3 != currentLine && messages.end() != currentMessage; ++currentLine, ++
		       currentMessage)
		{
			SetConsoleCursorPosition(_mainScreenBuffer, {0, currentLine});

			WriteConsole(_mainScreenBuffer, currentMessage->c_str(), currentMessage->size(), &written, nullptr);
		}
	}

	void GUIRenderer::ShowUserName(std::string userName)
	{
		static std::string emptyName(_mainScreenInfo.dwSize.X, ' ');

		DWORD written;

		SetConsoleCursorPosition(_mainScreenBuffer, {0, _mainScreenInfo.dwSize.Y - 2});
		WriteConsole(_mainScreenBuffer, emptyName.c_str(), emptyName.size(), &written, nullptr);

		SetConsoleCursorPosition(_mainScreenBuffer, {0, _mainScreenInfo.dwSize.Y - 2});
		WriteConsole(_mainScreenBuffer, userName.c_str(), userName.size(), &written, nullptr);
	}

	void GUIRenderer::ShowNetworkStatus(std::string newStatus)
	{
		static std::string emptyStatus(_mainScreenInfo.dwSize.X - _networkStatusLabel.length(), ' ');

		DWORD written;

		SetConsoleCursorPosition(_mainScreenBuffer, {(SHORT)_networkStatusLabel.length(), 0});
		WriteConsole(_mainScreenBuffer, emptyStatus.c_str(), emptyStatus.size(), &written, nullptr);

		SetConsoleCursorPosition(_mainScreenBuffer, {(SHORT)_networkStatusLabel.length(), 0});
		WriteConsole(_mainScreenBuffer, newStatus.c_str(), newStatus.size(), &written, nullptr);
	}
}
