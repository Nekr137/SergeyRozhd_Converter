#pragma once

#include <string>
#include <vector>
#include <memory>

struct Row {
	Row(std::string iHeader) : _header(std::move(iHeader))
	{
	}
	std::string _header;
	std::vector<std::string> _aRow;
};

typedef std::vector<Row> Rows;

