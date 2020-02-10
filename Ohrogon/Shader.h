#pragma once
#include "gl_core_4_5.h"
#include <fstream>
#include <string>
//#include <assert.h>
#include <sstream>
#include <iostream>

using uint = unsigned int;

class Shader{
	std::string VertSrc;
	std::string FragSrc;

	std::string LoadFile(const char* filename) {
		std::ifstream file(filename);
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

	uint CompileShader(std::string data, int flag) {
		const char* dataSrc = data.c_str();
		uint id = glCreateShader(flag);

		glShaderSource(id, 1, (const GLchar**)&dataSrc, 0);
		glCompileShader(id);

		GLint success = GL_FALSE;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* errorLog = new char[maxLength];
			glGetShaderInfoLog(id, maxLength, &maxLength, errorLog);

			printf(errorLog);
			free(errorLog);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(id); // Don't leak the shader.
			throw std::exception("Cannot Compile the Shader");

			return 0;
		}

		return id;
	}

public:
	uint VertID;
	uint FragID;

	uint ProgrammID;

	Shader(const char* VertFile, const char* FragFile){

		//Load Files
		VertSrc = LoadFile(VertFile);
		FragSrc = LoadFile(FragFile);


		//Compile Shaders
		VertID = CompileShader(VertSrc, GL_VERTEX_SHADER);
		FragID = CompileShader(FragSrc, GL_FRAGMENT_SHADER);


		//Create and Link Programm
		ProgrammID = glCreateProgram();

		glAttachShader(ProgrammID, VertID);
		glAttachShader(ProgrammID, FragID);

		glLinkProgram(ProgrammID);

		GLint success = GL_FALSE;
		glGetProgramiv(ProgrammID, GL_LINK_STATUS, &success);
		if(!success){

			GLint maxLength = 0;
			glGetProgramiv(ProgrammID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* errorLog = new char[maxLength];
			glGetProgramInfoLog(ProgrammID, maxLength, &maxLength, errorLog);
			
			//Print the Log and delete the memory
			printf(errorLog);
			free(errorLog);


			throw std::exception("Cannot Link Shaders");
		}
	}

	~Shader() = default;
};

