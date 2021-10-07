#pragma once
#include <atyp_Array.h>
#include <glad.h>

#include <map>

#include "File.h"

//#include <assert.h>

using uint = unsigned int;


enum class ShaderType: int {
	Vertex = GL_VERTEX_SHADER,
	Frag = GL_FRAGMENT_SHADER
};

const char* ShaderName(ShaderType type){
	switch(type){
		case ShaderType::Vertex:
			return "Vertex";
		case ShaderType::Frag:
			return "Fragment";
		default:
			return nullptr;
	}
}

class Shader{
	class src{
		public:
		uint id;
		const char* file;
		ShaderType type;

		src(const char* path, ShaderType type): file(path), type(type){
			auto data = File::ReadTextCStr(path);
			Console::Debug(String::format("Compiling %s Shader from %s", ShaderName(type), path));
			id = CompileSource(data, (int)type);
		}

		void Recompile(){
			glDeleteShader(id);
			id = CompileSource(File::ReadTextCStr(file), (int)type);
			Console::Debug(String::format("ReCompiling %s Shader from %s", ShaderName(type), file));
		}
	};

	static Array<src> Sources;

	Array<uint> sourceIDs;
	Array<uint> compiledIDs;

	static uint CompileSource(const char* data, GLint flag) {
		uint id = glCreateShader(flag);

		glShaderSource(id, 1, &data, nullptr);
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

			Console::Error(String::format("Error Compiling Shader: %s", errorLog));
			free(errorLog);

			// printf(dataSrc);

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

	uint ProgrammID;

	Shader(){

		//Create and Link Programm
		ProgrammID = glCreateProgram();

		glLinkProgram(ProgrammID);


	}

	~Shader(){
		glDeleteProgram(ProgrammID);
	}

	uint LoadShader(const char* filename, ShaderType shaderType){
		uint index = Sources.length;
		Sources.push(src(filename, shaderType));
		sourceIDs.push(index);
		return index;
	}

	void AddShader(uint id){
		sourceIDs.push(id);
	}

	uint CompileShader(){
		for (uint& id : sourceIDs) {
			glAttachShader(ProgrammID, Sources[id].id);
		}

		compiledIDs = sourceIDs.map<uint>([](const uint& e){return Sources[e].id;});

		glLinkProgram(ProgrammID);
		AssertShaderErrors(ProgrammID);

		return ProgrammID;
	}

	uint CompileShader(std::initializer_list<uint> ids){
		for (uint& id : sourceIDs) {
			glAttachShader(ProgrammID, Sources[id].id);
		}

		compiledIDs = sourceIDs.map<uint>([](const uint& e){return Sources[e].id;});

		auto ptr = ids.begin();
		while (ptr != ids.end()){
			glAttachShader(ProgrammID, Sources[*ptr].id);
			compiledIDs.push(Sources[*ptr].id);
		}
		glLinkProgram(ProgrammID);
		AssertShaderErrors(ProgrammID);



		return ProgrammID;
	}

	operator uint(){
		return ProgrammID;
	}

	void Reload(){
		for(uint& id: compiledIDs){
			glDetachShader(ProgrammID, id);
		}

		for(uint& id : sourceIDs){
			Sources[id].Recompile();
			glAttachShader(ProgrammID, Sources[id].id);
		}

		compiledIDs = sourceIDs.map<uint>([](const uint& e){return Sources[e].id;});


		glLinkProgram(ProgrammID);
		AssertShaderErrors(ProgrammID);
	}
};

Array<Shader::src> Shader::Sources = Array<Shader::src>();
