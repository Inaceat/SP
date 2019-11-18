#include "stdafx.h"


#include "ChategGUICommand.hpp"


namespace Chateg
{
	ChategGUICommand::ChategGUICommand(CommandType type, std::string data) :
		_type(type),
		_data(data)
	{}

	ChategGUICommand::CommandType ChategGUICommand::Type() const
	{
		return _type;
	}

	std::string ChategGUICommand::Data() const
	{
		return _data;
	}
}
