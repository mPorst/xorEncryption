#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include "encrypting.h"

#define DEBUG1

/***
XOR encryption by MAPster
ToDo:
include good random generator for long key generation
***/

/// public variables



int main(int argc, char** argv)
{
	std::string fileName;
	bool filenameSet=false; bool helpSet=false;

	parseCommandLine(argc, argv, &fileName, &filenameSet, &helpSet);
	if(helpSet == true)
	{
		return 0;  // quit after help was displayed
	}

	std::cout << "File encryption/decryption program using XOR algorithm" <<std::endl;
	std::string key;
	//std::string* longKey = new std::string();
	getString(&key);
//	longKey = lengthenKey(&key);
	
	if(filenameSet == false)
	{
		std::cout << "choose which file to encrypt" << std::endl;
		std::cin >> fileName;
	}	

	encryptFile(fileName, key);	
	std::cout << "operation succeeded" << std::endl;
return 0;
}

