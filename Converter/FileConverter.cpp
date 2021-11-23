#include <iostream>
#include <fstream>
#include <regex>

#include "FileConverter.h"


static std::vector<std::string> splitString(const std::string& iStr)
{
	std::regex r{ "[\\w\\d\\.\\,]+"};
	std::vector<std::string> aStr{
		std::sregex_token_iterator{iStr.begin(), iStr.end(), r, 0}, // Mark `0` here i.e. whole regex match
		std::sregex_token_iterator{}
	};
	for (std::string& s : aStr) {
		std::string replStr;
		std::regex_replace(std::back_inserter(replStr), s.begin(), s.end(), std::regex({ "\\," }), ".");
		s = std::move(replStr);
	}
	return aStr;
}

FileConverterPtr FileConverter::Create(std::string iFilename)
{
	return std::shared_ptr<FileConverter>(new FileConverter(std::move(iFilename)));
}


FileConverter::FileConverter(std::string iFilename)
	: _fname(std::move(iFilename)) {

	_fnameWithoutExt = _fname;
	auto delim = _fname.find_last_of(".");
	_fnameWithoutExt = _fname.substr(0, delim);
	_fnameExt = _fname.substr(delim + 1, _fname.back());
}

bool FileConverter::Parse()
{
	const std::string delim = "\t";

	std::ifstream inp;

	inp.open(_fname);
	if (!inp.is_open()) {
		std::cout << "[ Error ] Failed to load the " << _fname << " file." << std::endl;
		return false;
	}

	std::string line;

	// Parsing the header
	getline(inp, line);
	std::vector<std::string> aHeaders = splitString(line);
	_aRows.reserve(aHeaders.size());
	for (std::string& header : aHeaders)
		_aRows.emplace_back(std::move(header));

	// Parsing the data
	bool isHeaderPassed = false;
	for (size_t lineIdx = 1; lineIdx < 1e5; ++lineIdx) {

		if (!getline(inp, line))
			break;

		std::vector<std::string> aData;
		aData = splitString(line);

		isHeaderPassed |= !aData.empty();
		if (!isHeaderPassed)
			continue;

		if ((aData.size() != _aRows.size()) || (isHeaderPassed && aData.empty())) {
			std::cout << "[ Error ] Failed to read the line # " << lineIdx << ". Skip the line." << std::endl;
			continue;
		}
		for (size_t i = 0; i < _aRows.size(); ++i) {
			_aRows[i]._aRow.push_back(aData[i]);
		}	
	}
	return true;
}

bool FileConverter::Write(const std::vector<std::string>& iaOrder) const
{

	const std::string outputFilename = _fnameWithoutExt + "_proc." + _fnameExt;

	std::ofstream out;
	out.open(outputFilename);
	if (!out.is_open()) {
		std::cout << "[ Error ] Failed to open the output file named " + outputFilename;
		return false;
	}

	// Firstly, write headers as it is written in the `iaOrder`
	for (const std::string& headerInOrder : iaOrder) {
		for (size_t i = 0; i < _aRows.size(); ++i) {
			const std::string& header = _aRows[i]._header;
			if (header != headerInOrder)
				continue;

			out << header << "\t";
			for (const auto& elem : _aRows[i]._aRow) {
				out << elem << "\t";
			}
			out << std::endl;
		}
	}

	// Secondly, writing the data in the reversed order
	for (int i = _aRows.size() - 1, j = 1; i >= 0; --i) {
		bool isNumber =
			!_aRows[i]._header.empty() &&
			_aRows[i]._header.find_first_not_of("0123456789") == std::string::npos;
		if (!isNumber)
			break;
		out << j++ << "\t";
		for (const auto& elem : _aRows[i]._aRow) {
			out << elem << "\t";
		}
		out << std::endl;
	}

	return true;
}

void FileConverter::AppendTextToFname(std::string str)
{
	_fnameWithoutExt += std::move(str);
}