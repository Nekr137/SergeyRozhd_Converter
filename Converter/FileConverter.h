#pragma once

#include <string>
#include <vector>


class FileConverter
{
public:
	FileConverter(const std::string& iFilename);
	bool Run(const std::vector<std::string>& iaOrder);

private:
	bool parse();
	void rearrange(const std::vector<std::string>& iaOrder);
	void write();

	std::string _fname;
	std::vector<std::string> _aHeaders;
	std::vector<std::vector<std::string>> _aaData;
};

