#pragma once

#include <atyp_String.h>

#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

class File {
	static const char* fixUtf8(const char* str) {
		const int bufferSize = strlen(str) + 1;
		char* buffer = (char*)malloc(bufferSize);
		memset(buffer, 0, bufferSize);
		memcpy(buffer, str, bufferSize);

		// printf("First Character:%c\n", buffer[0]);

		for(int i = 0; i < bufferSize; ++i){
			char item = buffer[i];
			if(item > 127){
				Console::Error("Invalid Unicode Character Detected");
			}
		}

		return buffer;
	}

 public:
  static String ReadTextAlt(const char* filepath) {
    std::ifstream file(filepath);
    std::stringstream fileData;

    if (file.is_open() && file.good()) {
      fileData << file.rdbuf();

      const char* fileContent = fileData.str().c_str();

      file.close();

      return String(fileContent);
    } else {
      Console::Error(String::format("Loading of %s Failed.", filepath));
      throw "Could not Load File";
    }
  }
  static String ReadText(const char* filepath) {
    char* buffer = 0;
    long length;
    FILE* f = fopen(filepath, "rb");

    if (f) {
      fseek(f, 0, SEEK_END);
      length = ftell(f);
      fseek(f, 0, SEEK_SET);
      buffer = (char*)malloc(length);
      if (buffer) {
        fread(buffer, 1, length, f);
      }
      fclose(f);
    }

    if (buffer) {
      // start to process your data / extract strings here...
	  return String(buffer);
    }

	throw "Cannot Load File";
  }

  static const char* ReadTextCStr(const char* filepath) {
    char* buffer = 0;
    long length;
    FILE* f = fopen(filepath, "r");

    if (f) {
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = (char*)malloc(length + 1);
		memset(buffer, 0, length);
		if (buffer) {
			fread(buffer, 1, length, f);
		}
		buffer[length] = 0;
		fclose(f);
    }

    if (buffer) {
      // start to process your data / extract strings here...
      return fixUtf8(buffer);
    }

    throw "Cannot Load File";
  }
};
