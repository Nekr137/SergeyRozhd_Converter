#include <iostream>
#include <vector>


int main(int argc, const char* argv[]) {
	if (argc < 2) {
		std::cout << "[ Error ] Please specify at least one file to convert" << std::endl;
		return 0;
	}

	std::vector<std::string> aFilenames(argc - 1);
	for (int i = 1; i < argc; ++i) {
		aFilenames[i] = argv[i];
	}
}