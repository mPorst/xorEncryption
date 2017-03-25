#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include "boost/program_options.hpp"

#define DEBUG

/***
XOR encryption by MAPster
ToDo:
include good random generator for long key generation
***/

/// public variables

namespace po = boost::program_options;

void getString(std::string* key);
void encryptFile(std::string file, std::string key);
std::string* lengthenKey(std::string* key);
void deleteUnencrypted(std::string fileName);


int main(int argc, char** argv)
{
	std::string fileName;
	bool filenameSet=false;

	// program options part
		po::options_description desc("Allowed options");
		desc.add_options()
		("help", "XOR encryption/decryption program.")
		("filename", po::value<std::string>(), "file name")
		;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);    

		if (vm.count("help")) {
		std::cout << "Encryption and decryption with the XOR algorithm is the same process"
		<< "\n To decrypt, issue the same key as you used for encryption."
		<< std::endl
		<< "Here comes the usual help site: " << std::endl;
		std::cout << desc << "\n";
		return 1;
		}

		if (vm.count("filename")) {
		#ifdef DEBUG
			std::cout << "filename: " 
			<< vm["filename"].as<std::string>() << ".\n";
		#endif
			fileName = vm["filename"].as<std::string>();	
			filenameSet = true;
			
		} else {
		std::cout << "\n";
		}

	std::cout << "File encryption/decryption program using XOR algorithm" <<std::endl;
	std::string key;
	std::string* longKey = new std::string();
	getString(&key);
	longKey = lengthenKey(&key);
	std::cout << key << std::endl;
	std::cout << "generated key: " << *longKey << std::endl;
	if(filenameSet == false)
	{
		std::cout << "choose which file to encrypt" << std::endl;
		std::cin >> fileName;
	}	
	encryptFile(fileName, *longKey);	
	std::cout << "operation succeeded" << std::endl;
return 0;
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
	std::string* longKey = new std::string();
	std::string constantString1 = "TodayIsANiceDayThisEncryptionIsGoingToBeFine";
	if(key->size() > constantString1.size())
	{
		for(int i=0; i<constantString1.size(); i++)
		{
			longKey->push_back((key->at(i))^(constantString1.at(i)));
			longKey->push_back(key->at(i));
			longKey->push_back(constantString1.at(i));
		}
	}
	else
	{
		for(int i=0; i<key->size(); i++)
		{
			longKey->push_back((key->at(i))^(constantString1.at(i)));
			longKey->push_back(key->at(i));
			longKey->push_back(constantString1.at(i));
		}
	}
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
