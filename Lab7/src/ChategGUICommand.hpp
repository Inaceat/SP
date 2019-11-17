#pragma once


namespace Chateg
{
	class ChategGUICommand
	{
	public:
		enum class CommandType
		{
			Message,
			Quit
		};


	public:
		ChategGUICommand(CommandType type, std::string data);
	

		CommandType Type() const;

		std::string Data() const;


	private:
		CommandType _type;
	
		std::string _data;
		
	};
}