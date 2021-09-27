#include <sys/stat.h>
#include <iostream>

#include "FileConverter.h"


inline bool isFileExists(const std::string& iFilename)
{
	struct stat buffer;
	return (stat(iFilename.c_str(), &buffer) == 0);
}

FileConverter::FileConverter(const std::string& iFilename)
	: _fname(iFilename) {
}

bool FileConverter::Run(const std::vector<std::string>& iaOrder) {

	if (!parse())
		return false;

	rearrange(iaOrder);
	write();

	return true;
}

bool FileConverter::parse() {
	if (!isFileExists(_fname)) {
		std::cout << "[ Error ] Failed to load the " << _fname << " file." << std::endl;
		return false;
	}

	return true;
}

void FileConverter::rearrange(const std::vector<std::string>& iaOrder) {
}

void FileConverter::write() {
}