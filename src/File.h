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

      const char* fileContent = fileData.str().c_str();

      file.close();

	  printf(fileContent);

      return String(fileContent);
    } else {
      Console::Error(String::format("Loading of %s Failed.", filepath));
      throw "Could not Load File";
    }
  }
};
