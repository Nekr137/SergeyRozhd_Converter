#include <iostream>
#include <vector>

#include "FileConverter.h"


int main(int argc, const char* argv[]) {

	if (argc < 2) {
		std::cout << "Run:\n Converter.exe filename1 filename2 filename3 ... " << std::endl;
		return 0;
	}

	for (int i = 1; i < argc; ++i) {
		std::string fname = argv[i];
		FileConverter converter(fname);
		bool suc = converter.Run({ "Ens", /* "YR", "MO", */ "DA", "HH", "MM", "SS", "HH" });
		std::cout << (!suc ? "[ Error ] Failed to convert the " : "[ Ok ] Converted the ") << fname << " file." << std::endl;
	}

	return 0;
}

