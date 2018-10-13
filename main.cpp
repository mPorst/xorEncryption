#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include "encrypting.h"
#include "cmdLineParser.h"
#include <termios.h>
#include <unistd.h>

#define DEBUG1

/***
XOR encryption by MAPster
ToDo:
include good random generator for long key generation
***/

/// public variables



int main(int argc, char** argv)
{
	termios oldt;
	tcgetattr(STDIN_FILENO, &oldt);
	termios newt = oldt;
	newt.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	bool filenameSet=true;
	if(argc < 2)
		filenameSet=false;

	std::vector<std::string> arguments;
	std::string fileName;

	std::cout << "File encryption/decryption program using XOR algorithm" <<std::endl;
	std::string key;
	getString(&key);
	//restore terminal state
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	
	if(filenameSet==true)
	{
		arguments = readArguments(argc, argv);
		for(int i=1; i<argc; i++)
		{
			std::cout << "Now encrypting: " << arguments.at(i) << std::endl;
			encryptFile(arguments.at(i), key);	
		}
	}
	else
	{
		std::cout << "choose which file to encrypt" << std::endl;
		std::cin >> fileName;
		encryptFile(fileName, key);	
	}

	std::cout << "operation succeeded" << std::endl;
return 0;
}

