#include <fstream>
#include "utils.h"

std::string readBinFile(const char * const fname) {
	std::ifstream in(fname, std::ios::in | std::ios::binary);
	if (!in) {
		throw errno;
	}

	in.seekg(0, std::ios::end);
	std::string contents;
	contents.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&contents[0], contents.size());
	in.close();
	return contents;
}
