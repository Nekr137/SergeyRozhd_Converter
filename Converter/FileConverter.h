#pragma once

#include <string>
#include <vector>
#include <memory>

class FileConverter;
typedef std::shared_ptr<FileConverter> FileConverterPtr;

struct Row {
	Row(std::string iHeader) : _header(std::move(iHeader)) {}
	std::string _header;
	std::vector<std::string> _aRow;
};


class FileConverter
{
public:
	static FileConverterPtr Create(std::string iFilename);
	bool Parse();
	bool Write(const std::vector<std::string>& iaOrder) const;

	std::string GetFileName() const { return _fname; }

private:
	FileConverter(std::string iFilename);

	std::vector<Row> _aRows;

	std::string _fname;
	std::string _fnameWithoutExt;
	std::string _fnameExt;
};

