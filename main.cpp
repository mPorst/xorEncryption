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
	std::string key, key2;
    std ::cout << "----------" << std::endl;
	std::cout << "Now you are going to specify your key. You need to remember it in order to decrypt your files again." << std::endl
	<< "Anyway, do NOT save your key next to the encrypted files. This would make the encryption useless." <<std::endl
	<< "Please enter your key:" << std::endl;
	getString(&key);
	// get string second time
    std::cout << "Please enter your key a second time:" << std::endl;
    getString(&key2);
	//restore terminal state
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    if(key != key2)
    {
      std::cout << "Keys do not match. Abort" << std::endl;
      return 1;
    }
    // don't keep double copy of string in memory
    key2 = "dfhflesargfghrewhfaerhuflhesafkgfesaliufeafea43878956439875";
	
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

