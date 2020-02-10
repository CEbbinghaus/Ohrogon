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

public:
	uint VertID;
	uint FragID;

	uint ProgrammID;

	Shader(const char* VertFile, const char* FragFile){
		std::ifstream vert(VertFile);
		std::ifstream frag(FragFile);

		std::string line;
		std::stringstream file;

		//Vertex Shader File Loading
		if(vert.is_open()){
			file << vert.rdbuf();

			vert.close();

			VertSrc = file.str();
		} else
			throw std::exception("Vertex Shader Source File does not Exist");


		file.str("");

		//Fragment Shader File Loading
		if(frag.is_open()){
			file << frag.rdbuf();

			frag.close();

			FragSrc = file.str();
		} else
			throw std::exception("Fragment Shader Source File does not Exist");


		file.str("");

		//Vertex Shader Compalition
		
			const char* Vertdata = VertSrc.c_str();
			VertID = glCreateShader(GL_VERTEX_SHADER);

			glShaderSource(VertID, 1, (const GLchar**)&Vertdata, 0);
			glCompileShader(VertID);

			GLint success = GL_FALSE;
			glGetShaderiv(VertID, GL_COMPILE_STATUS, &success);

			if(success == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(VertID, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				char* errorLog = new char[maxLength];
				glGetShaderInfoLog(VertID, maxLength, &maxLength, errorLog);

				printf(errorLog);
				free(errorLog);

				// Provide the infolog in whatever manor you deem best.
				// Exit with failure.
				glDeleteShader(VertID); // Don't leak the shader.
				throw std::exception("Cannot Compile the Vertex Shader");
			}
		



		//Fragment Shader Compalition
		
			const char* Fragdata = FragSrc.c_str();
			FragID = glCreateShader(GL_FRAGMENT_SHADER);

			glShaderSource(FragID, 1, (const GLchar**)&Fragdata, 0);
			glCompileShader(FragID);

			success = GL_FALSE;
			glGetShaderiv(FragID, GL_COMPILE_STATUS, &success);

			if(success == GL_FALSE){
				GLint maxLength = 0;
				glGetShaderiv(FragID, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				char* errorLog = new char[maxLength];
				glGetShaderInfoLog(FragID, maxLength, &maxLength, errorLog);

				printf(errorLog);
				free(errorLog);

				// Provide the infolog in whatever manor you deem best.
				// Exit with failure.
				glDeleteShader(VertID); // Don't leak the shader.
				 
			}
		


		//Create and Link Programm
		ProgrammID = glCreateProgram();

		glAttachShader(ProgrammID, VertID);
		glAttachShader(ProgrammID, FragID);

		glLinkProgram(ProgrammID);

		success = GL_FALSE;
		glGetProgramiv(ProgrammID, GL_LINK_STATUS, &success);
		if(!success){

			GLint maxLength = 0;
			glGetProgramiv(ProgrammID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* errorLog = new char[maxLength];
			glGetProgramInfoLog(ProgrammID, maxLength, &maxLength, errorLog);

			printf(errorLog);
			free(errorLog);


			throw std::exception("Cannot Link Shaders");
		}
	}

	~Shader() = default;
};

