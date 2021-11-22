#include <iostream>
#include <vector>

#include "FileConverter.h"


int main(int argc, const char* argv[]) {

	if (argc < 2) {
		std::cout << "Run:\n Converter.exe filename1 filename2 filename3 ... " << std::endl;
		return 0;
	}

	std::vector<std::string> aHeaders = { "Ens", /* "YR", "MO", */ "DA", "HH", "MM", "SS", "HH" };

	std::vector<FileConverterPtr> apConverters;

	for (int i = 1; i < argc; ++i) {
		std::string fname = argv[i];
		apConverters.push_back(FileConverter::Create(fname));
	}

	for (FileConverterPtr& conv : apConverters) {
		if (!conv->Parse()) {
			std::cout << "[ Error ] Failed to parse file " << conv->GetFileName() << std::endl;
			return 0;
		}
	}

	if (apConverters.size() == 1) {
		const FileConverterPtr& conv = apConverters.front();
		bool suc = conv->Write(aHeaders);
		std::cout << (!suc ? "[ Error ] Failed to convert the " : "[ Ok ] Converted the ") << conv->GetFileName() << " file." << std::endl;
	}

	return 0;
}

