#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Row.h"

class FileConverter;
typedef std::shared_ptr<FileConverter> FileConverterPtr;


class FileConverter
{
public:
	static FileConverterPtr Create(std::string iFilename);
	bool Parse();
	bool Write(const std::vector<std::string>& iaOrder) const;

	std::string GetFileName() const { return _fname; }

	Rows& GetRows() { return _aRows; }
	const Rows& GetRows() const { return _aRows; }

	void AppendTextToFname(std::string str);

private:
	FileConverter(std::string iFilename);

	std::vector<Row> _aRows;

	std::string _fname;
	std::string _fnameWithoutExt;
	std::string _fnameExt;
};

