#include <iostream>
#include <fstream>

#include "FileConverter.h"


static std::vector<std::string> splitString(const std::string& iStr, const std::string& iDelim)
{
	std::vector<std::string> aSub;
	size_t start = 0;
	size_t end = iStr.find(iDelim);
	while (end != std::string::npos) {
		aSub.push_back(std::move(iStr.substr(start, end - start)));
		start = end + iDelim.length();
		end = iStr.find(iDelim, start);
	}
	std::string sub = iStr.substr(start, end - start);
	if (!sub.empty())
		aSub.push_back(std::move(sub));
	return aSub;
}

FileConverter::FileConverter(const std::string& iFilename)
	: _fname(iFilename) {

	_fnameWithoutExt = iFilename;
	auto delim = iFilename.find_last_of(".");
	_fnameWithoutExt = iFilename.substr(0, delim);
	_fnameExt = iFilename.substr(delim + 1, iFilename.back());
}

bool FileConverter::Run(const std::vector<std::string>& iaOrder) {

	const std::string delim = "\t";
	const std::string outputFilename = _fnameWithoutExt + "_proc." + _fnameExt;

	std::ofstream out;
	std::ifstream inp;

	inp.open(_fname);
	if (!inp.is_open()) {
		std::cout << "[ Error ] Failed to load the " << _fname << " file." << std::endl;
		return false;
	}

	std::string line;

	// Parsing the header
	std::vector<std::string> aHeaders;
	getline(inp, line);
	aHeaders = splitString(line, /* delimiter */ "\t");

	// Parsing the data
	std::vector<std::vector<std::string>> aaData;
	bool isHeaderPassed = false;
	for(size_t lineIdx = 1; lineIdx < 1e5; ++lineIdx) {

		if (!getline(inp, line))
			break;

		std::vector<std::string> aData;
		aData = splitString(line, /* delimiter */ "\t");

		isHeaderPassed |= !aData.empty();
		if (!isHeaderPassed)
			continue;

		if ((aData.size() != aHeaders.size()) || (isHeaderPassed && aData.empty())) {
			std::cout << "[ Error ] Failed to read the line # " << lineIdx << ". Skip the line." << std::endl;
			continue;
		}

		aaData.push_back(std::move(aData));
	}

	// Writing the data
	out.open(outputFilename);
	if (!out.is_open()) {
		std::cout << "[ Error ] Failed to open the output file named " + outputFilename;
		return false;
	}

	// Firstly, write headers as it is written in the `iaOrder`
	for (const std::string& headerInOrder : iaOrder) {
		for (size_t i = 0; i < aHeaders.size(); ++i) {
			const std::string& header = aHeaders[i];
			if (header != headerInOrder)
				continue;

			out << header << "\t";
			for (const auto& aData : aaData) {
				out << aData[i] << "\t";
			}
			out << std::endl;
		}
	}

	// Secondly, writing the data in the reversed order
	for (int i = aHeaders.size() - 1, j = 1; i >= 0; --i) {
		bool isNumber =
			!aHeaders[i].empty() &&
			 aHeaders[i].find_first_not_of("0123456789") == std::string::npos;
		if (!isNumber)
			break;
		out << j++ << "\t";
		for (const auto& aData : aaData) {
			out << aData[i] << "\t";
		}
		out << std::endl;
	}

	return true;
}

