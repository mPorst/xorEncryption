#include <string>


// prototypes
void getString(std::string* key);
void encryptFile(std::string file, std::string key);
std::string* lengthenKey(std::string* key, unsigned int fileLength);
void deleteUnencrypted(std::string fileName);
void parseCommandLine(int argc, char** argv, std::string* fileName, bool* filenameSet, bool* helpSet);
