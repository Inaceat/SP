#include "stdafx.h"


#include "Log.hpp"


std::string logFileName = "F:\\Projects\\SP\\Lab11\\data\\log.txt";


void Log(const std::string& message)
{
	std::fstream fs;
	fs.open(logFileName, std::ios_base::app);

	fs << message << std::endl;

	fs.close();
}
