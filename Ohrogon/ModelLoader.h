#pragma once
#include <atyp_Array.h>

#include <iostream>

#include "Mesh.h"
#include "File.h"

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

		while (*(data + offset) != 0 && *(data + offset) != '\n'){
			if (*(data + offset) == ' ')
				segments.push(data + offset);

			++offset;
		}

		return segments;
	}

public:
	static Mesh LoadObj(const char* path){
		auto file = File::ReadText(path);
		Array<const char*> lines;
		auto adr = file.begin();

		Array<Vector3> verts;
		Array<Vector3> normals;
		Array<uint> indices;

		lines.push(adr._Ptr);
		while (adr != file.end()) {
			if (*adr == '\n')
				lines.push(adr._Ptr + 1);

			++adr;
		}

		for (auto [lineptr, index] : lines){

			const char* line = *lineptr;

			if (startsWith(line, "#"))continue;

			if (startsWith(line, "v")){
				auto args = SplitString(line);
				if (args.length != 3) {
					#ifdef _DEBUG
						throw "Vertex Must have 3 Positions";
					#else			
						continue;
					#endif // __DEBUG__
				}
				float x =  std::stof(args[0]);
				float y =  std::stof(args[1]);
				float z =  std::stof(args[2]);
				Vector3 pos = Vector3(x, y, z);
				verts.push(pos);
			} else if (startsWith(line, "vn")) {
					auto args = SplitString(line);
					if (args.length != 3) {
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
			} else if (startsWith(line, "f")){
				auto args = SplitString(line);
				auto triangles = Triangulate(&args);
				for (Array<const char*>* triangle : triangles){
					for (const char* index : *triangle){
						auto vertArgs = SplitString(index, '/', false);
						uint vertIndex = std::stoi(vertArgs[0]);
						indices.push(vertIndex - 1);
					}
					delete triangle;
				}
			}
		}
	
		Mesh m = Mesh(verts, indices);
		m.SetNormals(normals);
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