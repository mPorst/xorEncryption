#include "fileHelper.h"
#include <fstream>

int FileSize(std::fstream* f)
{
	f->seekg(0, std::ios_base::beg);
	std::ifstream::pos_type begin_pos = f->tellg();
	f->seekg(0, std::ios_base::end);
	std::ifstream::pos_type end_pos = f->tellg();
	f->seekg(0, std::ios_base::beg); // need to return to beginning status
	return static_cast<int>(end_pos - begin_pos);
}
