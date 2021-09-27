#pragma once

#include <string>
#include <vector>


class FileConverter
{
public:
	FileConverter(const std::string& iFilename);
	bool Run(const std::vector<std::string>& iaOrder);

private:
	std::string _fname;
	std::string _fnameWithoutExt;
	std::string _fnameExt;
};

