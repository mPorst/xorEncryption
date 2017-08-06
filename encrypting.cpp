#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <random>
#include <fstream>
#include <boost/program_options.hpp>


#include "encrypting.h"

#define DEBUG1
//#define N 250000000
#define N 25

namespace po = boost::program_options; // boost namespace abbreviation

void parseCommandLine(int argc, char** argv, std::string* fileName, bool* filenameSet, bool* helpSet)
{
	
	// program options part
	// 
		po::options_description desc("Allowed options");
		desc.add_options()
		("help", "XOR encryption/decryption program.")
		("filename", po::value<std::string>(), "file name")
		;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);    

		if (vm.count("help")) {
		*helpSet = true;
		std::cout << "Encryption and decryption with the XOR algorithm is the same process"
		<< "\n To decrypt, issue the same key as you used for encryption."
		<< std::endl
		<< "Here comes the usual help site: " << std::endl;
		std::cout << desc << "\n";
		return;
		}

		if (vm.count("filename")) {
		#ifdef DEBUG
			std::cout << "filename: " 
			<< vm["filename"].as<std::string>() << ".\n";
		#endif
			*fileName = vm["filename"].as<std::string>();	
			*filenameSet = true;
			
		} else {
		std::cout << "\n";
		}
}

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
	
	std::fstream encryptedFile;
	encryptedFile.open(file.append(".tmp").c_str(), std::fstream::out); // file should be string in order to append sth like .enc
	file.erase(file.end()-4, file.end());

	#ifdef DEBUG
		std::cout << "DEBUG: filename after removing .tmp: " << file <<std::endl;
	#endif	
	char fileContent;
	std::string* temp = new std::string();

	unsigned int i = 0, j = 0;
	while(fileToEncrypt.get(fileContent))
	{
		if( j >= (key.size()) ) j=0; // loop key
			#ifdef DEBUG
				std::cout << j << std::endl;
			#endif
		temp->push_back(fileContent ^ key.at(j)); // XOR encryption
		encryptedFile << temp->at(i);
			#ifdef DEBUG
				std::cout << "temp(encrypted): " << temp->at(i) << 
				"\n file content: " << fileContent << 
				"\n key char: " << key.at(j) << std::endl;
			#endif
		++j; ++i;
	}
	encryptedFile.close();
	fileToEncrypt.close();
	deleteUnencrypted(file);
}	

std::string* lengthenKey(std::string* key)
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
	for(int i=0; i<(N); i++) //actual key generation //N should be determined by file size
	{
		temp = randomGenerator()%255;
		std::cout << "generated: " << temp << std::endl;
		longKey->push_back(temp); // here the digits get appended. how to tell c++ to append ASCII char ?
		std::cout << "longKey: " << longKey->at(i) << std::endl;
	}
	std::cout << "longKey has been created " << std::endl;

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
