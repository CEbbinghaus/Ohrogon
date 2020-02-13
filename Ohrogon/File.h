#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using string = std::string;

static class File {
public:
	static string ReadText(const char* filepath) {
		std::ifstream file(filepath);
		std::stringstream fileData;


		//Vertex Shader File Loading
		if (file.is_open()) {
			fileData << file.rdbuf();

			file.close();

			return fileData.str();
		}
		else
			throw "Could not Load File";
	}
};