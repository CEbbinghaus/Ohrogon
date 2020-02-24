#pragma once
#include <atyp_Array.h>

#include <iostream>

#include "Mesh.h"
#include "File.h"

#ifdef _DEBUG
	#define check(condition, text) \
	if (!condition)throw text
#else			
	#define check(condition, text)\
	continue
#endif

class ModelLoader {
	static bool startsWith(const char* str, const char* pre){
		return !strncmp(str, pre, strlen(pre));
	}

	static Array<Array<const char*>*> Triangulate(Array<const char*>* data) {

		if (data->length <= 3){
			return Array({ new Array(*data) });
		}
		else if (data->length == 4) {
			return Array({
				new Array({(*data)[0], (*data)[1], (*data)[2]}),
				new Array({(*data)[2], (*data)[3], (*data)[0]})
			});
		} else {
			Array<Array<const char*>*> result;
			for (int i = 1; i < data->length - 1; ++i){
				result.push(new Array({ (*data)[0], (*data)[i], (*data)[i + 1]}));
			}
			return result;
		}
	}

	static Array<const char*> SplitString(const char* data, char character = ' ', bool ignoreFirst = true){
		int offset = 0;

		Array<const char*> segments;

		if (!ignoreFirst)
			segments.push(data);

		while (*(data + offset) != 0 && *(data + offset) != '\n' && (character == ' ' ? true : (*(data + offset) != ' '))){
			if (*(data + offset) == character)
				segments.push(data + ++offset);

			++offset;
		}

		return segments;
	}

	static int Indexof(const char* string, const char* pattern) {
		int patternLength = strlen(pattern);
		int index = 0;

		int output = -1;
		do {

			if (!memcmp(string + index, pattern, sizeof(char) * patternLength))
				return index;

			++index;
		} while (string[index] != '\n' && string[index] != 0);
		return output;
	}

public:
	static Mesh LoadObj(const char* path){
		auto file = File::ReadText(path);
		Array<const char*> lines;
		auto adr = file.begin();

		Array<Vector3> vertexBuffer;
		Array<Vector3> normalBuffer;
		Array<Vector2> uvBuffer;

		Array<Vector3> VertexArray;
		Array<Vector3> NormalArray;
		Array<Vector2> UVArray;

		Array<uint> indices;

		lines.push(adr._Ptr);
		while (adr != file.end()) {
			if (*adr == '\n')
				lines.push(adr._Ptr + 1);

			++adr;
		}

		for (auto lineptr : lines){

			const char* line = *lineptr;

			if (startsWith(line, "#"))continue;

			if (startsWith(line, "vt")) {
				auto args = SplitString(line);

				check(args.length > 2, "UV Coordinate Must have at least 2 Arguments");

				float u = std::stof(args[0]);
				float v = std::stof(args[1]);

				uvBuffer.push(Vector2(u, v));

			}else if (startsWith(line, "vn")) {
				auto args = SplitString(line);

				check(args.length == 3, "Vertex Normal Requires Three Arguments");


				float x = std::stof(args[0]);
				float y = std::stof(args[1]);
				float z = std::stof(args[2]);

				normalBuffer.push(Vector3(x, y, z));


			} else if (startsWith(line, "v")) {
				auto args = SplitString(line);

				check(args.length == 3, "Vertex Position Requires Three Arguments");

				float x = std::stof(args[0]);
				float y = std::stof(args[1]);
				float z = std::stof(args[2]);


				vertexBuffer.push(Vector3(x, y, z));
			}else if (startsWith(line, "f")){

				auto args = SplitString(line);

				auto triangles = Triangulate(&args);


				for (Array<const char*>* triangle : triangles){
					for (const char* index : *triangle){

						bool doubleSlash = false;
						Array<const char*> indexArgs;


						if(Indexof(index, "//") == -1)
							indexArgs = SplitString(index, '/', false);
						else {
							doubleSlash = true;
							indexArgs = SplitString(index, '//', false);
						}
						
						check(indexArgs.length > 0, "Indicies Must have a Minimum of 1 Argument");

						switch (indexArgs.length) {
							case 1:
								indices.push(std::stoi(indexArgs[0]) - 1);
								break;
							case 2:
								indices.push(indices.length);
								VertexArray.push(vertexBuffer[std::stoi(indexArgs[0]) - 1]);
								if (doubleSlash)
									NormalArray.push(normalBuffer[std::stoi(indexArgs[1]) - 1]);
								else
									UVArray.push(uvBuffer[std::stoi(indexArgs[1]) - 1]);
								break;
							case 3:
								indices.push(indices.length);
								VertexArray.push(vertexBuffer[std::stoi(indexArgs[0]) - 1]);
								UVArray.push(uvBuffer[std::stoi(indexArgs[1]) - 1]);
								NormalArray.push(normalBuffer[std::stoi(indexArgs[2]) - 1]);
								break;

						}
						
					}
					delete triangle;
				}
			}
		}
	
		if(VertexArray.length == 0)
			VertexArray = vertexBuffer;

		Mesh m = Mesh();
		m.SetVertices(VertexArray);
		m.SetIndices(indices);
		m.SetNormals(NormalArray);
		m.SetUVs(UVArray);
		return m;
	}
	
	static MeshData LoadRawObj(const char* path){
		auto file = File::ReadText(path);
		Array<const char*> lines;
		auto adr = file.begin();

		Array<Vector3> verts;
		Array<Vector3> normals;
		Array<uint> indices;

		lines.push(adr._Ptr);
		while(adr != file.end()){
			if(*adr == '\n')
				lines.push(adr._Ptr + 1);

			++adr;
		}

		for(auto [lineptr, index] : lines){

			const char* line = *lineptr;

			if(startsWith(line, "#"))continue;

			if(startsWith(line, "v")){
				auto args = SplitString(line);
				if(args.length != 3){
				#ifdef _DEBUG
					throw "Vertex Must have 3 Positions";
				#else			
					continue;
				#endif // __DEBUG__
				}
				float x = std::stof(args[0]);
				float y = std::stof(args[1]);
				float z = std::stof(args[2]);
				Vector3 pos = Vector3(x, y, z);
				verts.push(pos);
			} else if(startsWith(line, "vn")){
				auto args = SplitString(line);
				if(args.length != 3){
				#ifdef _DEBUG
					throw "Vertex Must have 3 Positions";
				#else			
					continue;
				#endif // __DEBUG__
				}
				float x = std::stof(args[0]);
				float y = std::stof(args[1]);
				float z = std::stof(args[2]);
				Vector3 pos = Vector3(x, y, z);
				normals.push(pos);
			} else if(startsWith(line, "f")){
				auto args = SplitString(line);
				auto triangles = Triangulate(&args);
				for(Array<const char*>* triangle : triangles){
					for(const char* index : *triangle){
						auto vertArgs = SplitString(index, '/', false);
						uint vertIndex = std::stoi(vertArgs[0]);
						indices.push(vertIndex - 1);
					}
					delete triangle;
				}
			}
		}

		MeshData m;
		m.Vertices = verts;
		m.Indices = indices;
		m.Normals = normals;
		return m;
	}
};