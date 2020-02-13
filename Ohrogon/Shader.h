#pragma once
#include "gl_core_4_5.h"
#include <atyp_Array.h>

#include <fstream>
#include <string>
//#include <assert.h>
#include <sstream>
#include <iostream>

using uint = unsigned int;
using string = std::string;

class Shader{
	static string LoadFile(const char* filename) {
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

	static uint CompileSource(string data, GLint flag) {
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

	static void AssertShaderErrors(uint ProgrammID){
		GLint success = GL_FALSE;
		glGetProgramiv(ProgrammID, GL_LINK_STATUS, &success);
		if (!success) {

			GLint maxLength = 0;
			glGetProgramiv(ProgrammID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* errorLog = new char[maxLength];
			glGetProgramInfoLog(ProgrammID, maxLength, &maxLength, errorLog);

			//Print the Log and delete the memory
			printf(errorLog);
			delete[] errorLog;


			throw std::exception("Cannot Link Shaders");
		}
	}

public:
	enum class Type {
		Vertex = GL_VERTEX_SHADER,
		Frag = GL_FRAGMENT_SHADER
	};

	Array<uint> ShaderIDs;

	uint ProgrammID;

	Shader(){

		//Create and Link Programm
		ProgrammID = glCreateProgram();

		glLinkProgram(ProgrammID);

		
	}

	//static uint LoadShader(const char* filename, Type ShaderType) {
	//	string source = LoadFile(filename);
	//	uint id = CompileSource(source, (GLint)ShaderType);
	//	return id;
	//}

	uint LoadShader(const char* filename, Type ShaderType){
		string source = LoadFile(filename);
		uint id = CompileSource(source, (GLint)ShaderType);
		ShaderIDs.push(id);
		return id;
	}
	
	uint AddShader(uint id){
		ShaderIDs.push(id);
	}

	uint CompileShader(){
		for (uint& id : ShaderIDs) {
			glAttachShader(ProgrammID, id);
		}
		glLinkProgram(ProgrammID);
		AssertShaderErrors(ProgrammID);

		return ProgrammID;
	}

	uint CompileShader(std::initializer_list<uint> ids){
		for (uint& id : ShaderIDs) {
			glAttachShader(ProgrammID, id);
		}

		auto ptr = ids.begin();
		while (ptr != ids.end()){
			glAttachShader(ProgrammID, *ptr);
		}
		glLinkProgram(ProgrammID);
		AssertShaderErrors(ProgrammID);

		return ProgrammID;
	}

	//static uint CompileShader(std::initializer_list<uint> ids){
	//	uint ProgrammID = glCreateProgram();
	//	auto ptr = ids.begin();
	//	while (ptr != ids.end()) {
	//		glAttachShader(ProgrammID, *ptr);
	//	}
	//	glLinkProgram(ProgrammID);
	//	AssertShaderErrors(ProgrammID);
	//}

	operator uint(){
		return ProgrammID;
	}

	~Shader() = default;
};

