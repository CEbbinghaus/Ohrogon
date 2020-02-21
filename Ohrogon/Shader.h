#pragma once
#include "gl_core_4_5.h"
#include <atyp_Array.h>

#include "File.h"
//#include <assert.h>

using string = std::string;
using uint = unsigned int;

class Shader{

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
			throw "Cannot Compile the Shader";

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


			throw "Cannot Link Shaders";
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

	~Shader(){
		glDeleteProgram(ProgrammID);
	}

	uint LoadShader(const char* filename, Type ShaderType){
		string source = File::ReadText(filename);
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

	operator uint(){
		return ProgrammID;
	}
};

