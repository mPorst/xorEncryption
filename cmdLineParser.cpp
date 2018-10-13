#include "cmdLineParser.h"

std::vector<std::string> readArguments(int argc, char** argv)
{
	std::vector<std::string> arguments;
	for(int i=0; i<argc; i++)
	{
		arguments.push_back(argv[i]);
	}
	return arguments;
}
