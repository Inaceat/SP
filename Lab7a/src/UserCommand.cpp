#include "stdafx.h"


#include "UserCommand.hpp"


namespace TTT
{
	UserCommand::UserCommand(Type type, std::string data):
		_type(type),
		_data(data)
	{
	}


	UserCommand::Type UserCommand::GetType() const
	{
		return _type;
	}

	std::string UserCommand::GetData() const
	{
		return _data;
	}
}
