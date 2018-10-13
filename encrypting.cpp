#pragma once

#include "fileHelper.h"
#include <string>
#include <iostream>
#include <sstream>
#include <random>
#include <fstream>


#include "encrypting.h"

#undef DEBUG
#undef DEBUG1
//#define N 250000000
//#define N 2500

void getString(std::string* key)
{
	std::cout << "Now you are going to specify your key. You need to remember it in order to decrypt your files again." << std::endl
	<< "Anyway, do NOT save your key next to the encrypted files. This would make the encryption useless." <<std::endl
	<< "Please enter your key:" << std::endl;
	std::cin >> *key;
}

void encryptFile(std::string file, std::string key)
{
	std::fstream fileToEncrypt;
	fileToEncrypt.open(file.c_str(), std::fstream::in);
	unsigned int fileLength = FileSize(&fileToEncrypt);

	std::string* longKey = lengthenKey(&key, fileLength);

	std::fstream encryptedFile;
	encryptedFile.open(file.append(".tmp").c_str(), std::fstream::out); // file should be string in order to append sth like .enc
	file.erase(file.end()-4, file.end());

	#ifdef DEBUG
		std::cout << "DEBUG: filename after removing .tmp: " << file <<std::endl;
	#endif	
	char fileContent;
	std::string* temp = new std::string();

	unsigned int i = 0, j = 0, k = 0;
	while(fileToEncrypt.get(fileContent))
	{
		if( j >= (longKey->size()) ) j=0; // loop long key
		if( k >= (key.size()) ) k=0; // loop short key

		temp->push_back(fileContent ^ longKey->at(j) ^ key.at(k)); // XOR encryption
		encryptedFile << temp->at(i);
			#ifdef DEBUG
				std::cout << "temp(encrypted): " << temp->at(i) << 
				"\n file content: " << fileContent << 
				"\n key char: " << longKey->at(j) << std::endl;
			#endif
		++j; ++i; ++k;
	}
	encryptedFile.close();
	fileToEncrypt.close();
	deleteUnencrypted(file);
}	

std::string* lengthenKey(std::string* key, unsigned int fileLength)
{
	std::string seed;
	std::ostringstream oss;

	for(unsigned int i=0; i<(key->size()); i++) // write key to string stream
	{
		oss << (int)key->at(i);  // 1 letter gets converted to a number yyy where y is a decimal digit (ASCII code)
	}

	seed = oss.str();
	std::string::size_type sz;

	while(seed.size() > 9) // cut off digits from key for seed creation (bad)
	{
		seed.erase(seed.begin(), seed.begin()+1);
		std::cout << seed << std::endl;
	}

	int iSeed = std::stoi(seed, &sz); // iSeed is now the appended y's (see comments before) but cut off to a maximum of 9 digits (so it does not exceed 2**32-1

	std::mersenne_twister_engine<std::uint_fast32_t, 32, 644, 397, 31,
                             0x9908b0df, 11,
                             0xffffffff, 7,
                             0x9d2c5680, 15,
                             0xefc60000, 18, 1812433253> randomGenerator(iSeed);

	std::string* longKey = new std::string();
	char temp;
	//get file size:
	//
	

	for(unsigned int i=0; i<(fileLength); i++) //actual key generation 
	{
		temp = randomGenerator()%255;
		//std::cout << "generated: " << temp << std::endl;
		longKey->push_back(temp);
		//std::cout << "longKey: " << longKey->at(i) << std::endl;
	}
	#ifdef DEBUG
	std::cout << "Your entered key: " << *key << " has size " << key->size()  << std::endl;
	std::cout << "The created longKey has size " << longKey->size() << std::endl;
	std::cout << "longKey has been created " << std::endl;
	#endif

	#ifdef DEBUG1
	for(unsigned int i=0; i<seed.size(); i++)
	{
		std::cout << i <<": " << seed.at(i) << std::endl;
	}
	std::cout << seed.at(3) << std::endl;
	std::cout << seed.at(3) + 122 << std::endl;
	#endif

	return longKey;
}

void deleteUnencrypted(std::string fileName)
{
	std::fstream remainingPlaintextFile;
	remainingPlaintextFile.open(fileName.c_str(), std::fstream::out);
	
	std::fstream encryptedFile;
	encryptedFile.open(fileName.append(".tmp").c_str(),std::fstream::in);
	fileName.erase(fileName.end()-4, fileName.end());

	#ifdef DEBUG
		std::cout << "inside of deleteUnencrypted. content's going to be flushed." << std::endl;
		std::cout << "filename: " << fileName <<std::endl;
	#endif
	char fileContent;
	remainingPlaintextFile.flush();
	while(encryptedFile.get(fileContent))
	{
		remainingPlaintextFile << fileContent;	
	}
	remainingPlaintextFile.close();
	encryptedFile.close();
	remove(fileName.append(".tmp").c_str());
	fileName.erase(fileName.end()-4, fileName.end());
}
