#include "stdafx.h"


#include "ChategGUI.hpp"


namespace Chateg
{
	ChategGUI::ChategGUI()
	{
		CreatePipe(&_commandOut, &_commandIn, nullptr, 0);
		CreatePipe(&_letterOut, &_letterIn, nullptr, 0);
		CreatePipe(&_messageOut, &_messageIn, nullptr, 0);
	}
	
	ChategGUI::~ChategGUI()
	{
		//TODO send msg?
		_guiInputThread.join();
		_guiOutputThread.join();

		CloseHandle(_letterIn);
		CloseHandle(_letterOut);
		
		CloseHandle(_commandIn);
		CloseHandle(_commandOut);
		
		CloseHandle(_messageIn);
		CloseHandle(_messageOut);
	}
	
	
	void ChategGUI::Start()
	{
		_guiInputThread = std::thread( [this](){ this->ConsoleReader(); } );
		
		_guiOutputThread = std::thread( [this](){ this->ConsoleWriter(); } );
	}
	
	
	std::string ChategGUI::AskClientName()
	{
		return std::string("xXx_MegAboSs_xXx");
	}
	
	
	ChategGUICommand* ChategGUI::CommandReceive(int timeout)
	{
		DWORD bytesRead;
		DWORD bytesAvailable;

		PeekNamedPipe(_commandOut, nullptr, 0, nullptr, &bytesAvailable, nullptr);

		if (bytesAvailable <= 2)//less than size word
			Sleep(timeout);

		PeekNamedPipe(_commandOut, nullptr, 0, nullptr, &bytesAvailable, nullptr);

		if (bytesAvailable <= 2)
			return nullptr;


		char* buffer = new char[bytesAvailable];

		ReadFile(_commandOut, buffer, bytesAvailable, &bytesRead, nullptr);

		std::string message(buffer + 2);


		return new ChategGUICommand(ChategGUICommand::CommandType::Message, message);
	}
	
	
	void ChategGUI::ShowMessage(std::string messageText)
	{
		DWORD bytesWritten;

		WORD bufferSize = messageText.size() + 1 + 2;//text + '\0' + two bytes for size
		char* buffer = new char[bufferSize];

		memcpy_s(buffer, 2, &bufferSize, 2);//Copy size
		strcpy_s(buffer + 2, bufferSize - 2, messageText.c_str());//And text

		WriteFile(_messageIn, buffer, bufferSize, &bytesWritten, nullptr);

		delete[] buffer;
	}



	void ChategGUI::ConsoleWriter()
	{
		HANDLE consoleOut = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_SCREEN_BUFFER_INFO screenInfo;
		GetConsoleScreenBufferInfo(consoleOut, &screenInfo);

		COORD cursorPositionInput = { 0, 30 };//screenInfo.dwCursorPosition; //{ 0, 30 };
		COORD cursorPositionOutput = { 0, 6 };// screenInfo.dwCursorPosition;

		std::string currentMessageBuffer = "";

		while (true)
		{
			DWORD bytesRead;
			DWORD bytesAvailable;

			PeekNamedPipe(_letterOut, nullptr, 0, nullptr, &bytesAvailable, nullptr);

			if (0 != bytesAvailable)//Has bytes in letter pipe
			{
				WORD key;
				ReadFile(_letterOut, &key, sizeof(WORD), &bytesRead, nullptr);

				//If letter from [A-Z]
				if (0x41 <= key && key <= 0x5A)
				{
					//Add it to message
					currentMessageBuffer.push_back(static_cast<char>(key));

					cursorPositionInput.X = 0;
					SetConsoleCursorPosition(consoleOut, cursorPositionInput);

					std::cout << currentMessageBuffer;
				}
				//If enter
				else if (VK_RETURN == key)
				{
					currentMessageBuffer.replace(0, currentMessageBuffer.size(), currentMessageBuffer.size(), ' ');

					cursorPositionInput.X = 0;
					SetConsoleCursorPosition(consoleOut, cursorPositionInput);

					std::cout << currentMessageBuffer;

					currentMessageBuffer.erase();
				}
				//If backspace
				else if (VK_BACK == key)
				{
					if (currentMessageBuffer.empty())
						continue;

					currentMessageBuffer.pop_back();
					currentMessageBuffer.push_back(' ');

					cursorPositionInput.X = 0;
					SetConsoleCursorPosition(consoleOut, cursorPositionInput);

					std::cout << currentMessageBuffer;

					currentMessageBuffer.pop_back();
				}
				//If Exit key
				else if (VK_ESCAPE == key)
				{
					break;
				}
			}

			PeekNamedPipe(_messageOut, nullptr, 0, nullptr, &bytesAvailable, nullptr);

			if (bytesAvailable > 2)
			{
				char* buffer = new char[bytesAvailable];

				ReadFile(_messageOut, buffer, bytesAvailable, &bytesRead, nullptr);

				std::string message(buffer + 2);

				SetConsoleCursorPosition(consoleOut, cursorPositionOutput);

				std::cout << message << std::endl;

				cursorPositionOutput.Y++;
			}

		}
	}

	void ChategGUI::ConsoleReader()
	{
		HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
		INPUT_RECORD inputRecord;
		DWORD readEvents;


		DWORD bytesWritten;

		std::string currentMessageBuffer = "";

		while (true)
		{
			ReadConsoleInput(inputHandle, &inputRecord, 1, &readEvents);
			
			if (0 != readEvents && KEY_EVENT == inputRecord.EventType && inputRecord.Event.KeyEvent.bKeyDown)
			{
				//If letter from [A-Z]
				if (0x41 <= inputRecord.Event.KeyEvent.wVirtualKeyCode
				 && inputRecord.Event.KeyEvent.wVirtualKeyCode <= 0x5A)
				{
					currentMessageBuffer.push_back((char)inputRecord.Event.KeyEvent.wVirtualKeyCode);

					WriteFile(_letterIn, &(inputRecord.Event.KeyEvent.wVirtualKeyCode), sizeof(WORD), &bytesWritten, nullptr);
				}
				//If enter
				else if (VK_RETURN == inputRecord.Event.KeyEvent.wVirtualKeyCode)
				{
					if (currentMessageBuffer.empty())
						continue;

					//Clear writer
					WriteFile(_letterIn, &(inputRecord.Event.KeyEvent.wVirtualKeyCode), sizeof(WORD), &bytesWritten, nullptr);

					//Send to net
					WORD bufferSize = currentMessageBuffer.size() + 1 + 2;//text + '\0' + two bytes for size
					char* buffer = new char[bufferSize];

					memcpy_s(buffer, 2, &bufferSize, 2);//Copy size
					strcpy_s(buffer + 2, bufferSize - 2, currentMessageBuffer.c_str());//And text

					WriteFile(_commandIn, buffer, bufferSize, &bytesWritten, nullptr);

					delete[] buffer;
				}
				//If backspace
				else if (VK_BACK == inputRecord.Event.KeyEvent.wVirtualKeyCode)
				{
					if (currentMessageBuffer.empty())
						continue;

					currentMessageBuffer.pop_back();

					WriteFile(_letterIn, &(inputRecord.Event.KeyEvent.wVirtualKeyCode), sizeof(WORD), &bytesWritten, nullptr);
				}
				//If Exit key
				else if (VK_ESCAPE == inputRecord.Event.KeyEvent.wVirtualKeyCode)
				{
					WriteFile(_letterIn, &(inputRecord.Event.KeyEvent.wVirtualKeyCode), sizeof(WORD), &bytesWritten, nullptr);
					break;
				}
			}
		}
	}
}
