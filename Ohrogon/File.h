#pragma once

#include <atyp_String.h>
#include <fstream>
#include <iostream>
#include <sstream>

class File {
public:
	static String ReadText(const char* filepath) {
		std::ifstream file(filepath);
		std::stringstream fileData;

		if (file.is_open() && file.good()) {
			fileData << file.rdbuf();

			file.close();

			return String(fileData.str().c_str());
		}
		else
			throw "Could not Load File";
	}
};