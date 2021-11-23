#include <iostream>
#include <vector>

#include "FileConverter.h"
#include "Row.h"


int main(int argc, const char* argv[]) {

	if (argc != 2 && argc != 3) {
		std::cout << "Run:\n Converter.exe fname1 to convert from columnst to rows " << std::endl;
		std::cout << "Or Run:\n Converter.exe fDirection fMagnitude to convert from columns to rows and change velocities to U,V" << std::endl;
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
		return 0;
	}

	if (apConverters.size() == 2) {
		Rows& f1Rows = apConverters[0]->GetRows();
		Rows& f2Rows = apConverters[1]->GetRows();
		if (f1Rows.size() != f2Rows.size()) {
			std::cout << "[ Error ] Size problem" << std::endl;
			return 0;
		}
		if (f1Rows.front()._aRow.size() != f2Rows.front()._aRow.size()) {
			std::cout << "[ Error ] Row length problem" << std::endl;
			return 0;
		}

		auto convert = [] (Row& r1, Row&r2) {
			for (size_t i = 0; i < r1._aRow.size(); i++) {
				std::string& el1 = r1._aRow[i];
				std::string& el2 = r2._aRow[i];
				double val1 = std::stod(el1); // direction
				double val2 = std::stod(el2); // magnitude
				val1 *= 1e-3; // mm to m
				val2 *= 1e-3; // mm to m
				double u = std::cos(val1)*val2;
				double v = std::sin(val1)*val2;
				el1 = std::to_string(u);
				el2 = std::to_string(v);
			}
		};

		auto isNumber = [] (const std::string& iStr) -> bool {
			return !iStr.empty() && iStr.find_first_not_of("0123456789") == std::string::npos;
		};

		for (size_t row1Idx = 0; row1Idx < f1Rows.size(); ++row1Idx) {
			Row& row1 = f1Rows[row1Idx];
			if (!isNumber(row1._header))
				continue;

			for (size_t row2Idx = 0; row2Idx < f2Rows.size(); ++row2Idx) {
				Row& row2 = f2Rows[row2Idx];
				if (row1._header != row2._header) {
					if (row2Idx + 1 == f2Rows.size()) {
						std::cout << "[ Error ] Failed to find the same row in file2 (" << row1._header << ")" << std::endl;
						return 0;
					}
					continue;
				}
				convert(row1, row2);
				break;
			}
		}

		apConverters[0]->AppendTextToFname("_velocity_U_part");
		apConverters[1]->AppendTextToFname("_velocity_V_part");

		bool suc1 = apConverters[0]->Write(aHeaders);
		bool suc2 = apConverters[1]->Write(aHeaders);

		std::cout << (!suc1 ? "[ Error ] Failed to convert the " : "[ Ok ] Converted the ") << apConverters[0]->GetFileName() << " file." << std::endl;
		std::cout << (!suc2 ? "[ Error ] Failed to convert the " : "[ Ok ] Converted the ") << apConverters[0]->GetFileName() << " file." << std::endl;
	}

	return 0;
}

