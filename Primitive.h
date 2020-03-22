#pragma once

#include <atyp_Array.h>
#include <atyp_Vector3.h>
#include <atyp_Math.h>
#include "Mesh.h"

class Primitive {
public:
	static struct{
		static MeshData Plane(uint faces){
			Array<Vector3> verts = Array<Vector3>((faces + 1) * (faces + 1));

			for(int x = 0; x <= faces; ++x){
				for(int y = 0; y <= faces; ++y){
					verts[x * (faces + 1) + y] = Vector3((x / (float)faces) - 0.5f, 0.0f, (y / (float)faces) - 0.5f);
				}
			}


			uint indexSize = faces * faces * 6;
			Array<uint> indices = Array<uint>(indexSize);

			int index = 0;
			for(int x = 0; x < faces; ++x){
				for(int y = 0; y < faces; ++y){
					int nextLine = (faces + 1);
					uint position = (x * nextLine + y);

					indices[index++] = position;
					indices[index++] = position + 1;
					indices[index++] = position + nextLine;

					indices[index++] = position + 1;
					indices[index++] = position + nextLine + 1;
					indices[index++] = position + nextLine;
				}
			}
			MeshData m;
			m.Vertices = verts;
			m.Indices = indices;
			return m;
		}
	
		static MeshData Cylinder(int slices){
			Array<Vector3> vertices;
			Array<uint> indices;

			vertices.push(Vector3(0.0f, 0.5f, 0.0f));
			vertices.push(Vector3(0.0f, -0.5f, 0.0f));

			for(int i = 0; i < slices; i++){
				float theta = Math::TAO() * ((float)i / slices);

				vertices.push(Vector3(cos(theta) * 0.5f, 0.5f, sin(theta) * 0.5f));
				vertices.push(Vector3(cos(theta) * 0.5f, -0.5f, sin(theta) * 0.5f));
			}

			for(int i = 2; i < vertices.size() - 2; i += 2){
				//TODO: Cleanup with Initializer Lists
				indices.push(i + 2);
				indices.push(i);
				indices.push(0);

				indices.push(i + 3);
				indices.push(1);
				indices.push(i + 1);

				indices.push(i);
				indices.push(i + 2);
				indices.push(i + 1);

				indices.push(i + 2);
				indices.push(i + 3);
				indices.push(i + 1);
			}

			indices.push(2);
			indices.push(vertices.size() - 2);
			indices.push(0);

			indices.push(1);
			indices.push(vertices.size() - 1);
			indices.push(3);

			indices.push(vertices.size() - 2);
			indices.push(2);
			indices.push(vertices.size() - 1);

			indices.push(2);
			indices.push(3);
			indices.push(vertices.size() - 1);
			MeshData m;
			m.Vertices = vertices;
			m.Indices = indices;
			return m;
		}
	
		static MeshData Cube(){
			Array<Vector3> vertices = {
				Vector3(0.5f, 0.5f, -0.5f),  // front top right
				Vector3(0.5f, -0.5f, -0.5f),  // front bottom right
				Vector3(-0.5f, -0.5f, -0.5f),  // front bottom left
				Vector3(-0.5f, 0.5f, -0.5f),  // front top left 

				Vector3(0.5f, 0.5f, 0.5f),  // back top right
				Vector3(0.5f, -0.5f, 0.5f),  // back bottom right
				Vector3(-0.5f, -0.5f, 0.5f),  // back bottom left
				Vector3(-0.5f, 0.5f, 0.5f)   // back top left 
			};

			Array<uint> indices = {
				0, 1, 3,   // front first triangle
				1, 2, 3,   // front second triangle

				4, 7, 5,   // back first triangle
				5, 7, 6,    // back second triangle

				4, 5, 1,   // right first triangle
				1, 0, 4,    // right second triangle

				3, 2, 6,   // left first triangle
				6, 7, 3,    // left second triangle

				0, 3, 7,   // top first triangle
				7, 4, 0,    // top second triangle

				1, 6, 2,   // bottom first triangle
				6, 1, 5    // bottom second triangle
			};

			Array<Vector2> uvs = {
				Vector2(0.0f, 0.0f),
				Vector2(1.0f, 0.0f),
				Vector2(0.0f, 1.0f),
				Vector2(1.0f, 1.0f),
				
				Vector2(0.0f, 1.0f),
				Vector2(1.0f, 1.0f),
				Vector2(1.0f, 0.0f),
				Vector2(0.0f, 0.0f)
			};

			MeshData m;
			m.Vertices = vertices;
			m.Indices = indices;
			m.UVs = uvs;
			return m;
		}

		static MeshData Sphere(int RadiusSlices, int HeightSlices){
			RadiusSlices = Math::clamp(RadiusSlices, 3, INT_MAX);
			HeightSlices = Math::clamp(HeightSlices, 3, INT_MAX);

			MeshData mesh;

			mesh.Vertices.push(Vector3(0.0f, .5f, 0.0f));

			for(int h = 1; h < HeightSlices; ++h){
				for(int w = 0; w < RadiusSlices; ++w){
					float height = 1 - (h / (float)HeightSlices);
					float radius = w / (float)RadiusSlices;


					float x = sinf(radius * Math::TAO());
					float z = cosf(radius * Math::TAO());
					float r = sinf(height * Math::PI()) * .5f;

					mesh.Vertices.push(Vector3(x * r, height - 0.5f, z * r));
				}
			}
			mesh.Vertices.push(Vector3(0.0f, -.5f, 0.0f));
			
			mesh.Vertices.forEach([&mesh](Vector3& e) {
				mesh.UVs.push(Vector2(1.0f, 1.0f));
				e.normalise();
			});
 
			uint lastVertex = mesh.Vertices.length - 1;

			mesh.Indices.push({0, (uint)RadiusSlices, 1});
			mesh.Indices.push({lastVertex, lastVertex - RadiusSlices, lastVertex - 1});
			for(uint i = 1; i < RadiusSlices; ++i){
				mesh.Indices.push({0, i, i + 1});
				mesh.Indices.push({lastVertex, lastVertex - i, lastVertex - (i + 1)});
			}

			for(int h = 0; h < HeightSlices - 2; ++h){

				uint index = h * RadiusSlices + 1;

				// mesh.Indices.push({index + ((uint)RadiusSlices - 1), index, index + RadiusSlices});
				// mesh.Indices.push({index + ((uint)RadiusSlices - 1), index + RadiusSlices, index + RadiusSlices + 1});
				for(int w = 0; w < RadiusSlices - 1; ++w){
					uint rindex = index + w;
					mesh.Indices.push({rindex, rindex + RadiusSlices, rindex + 1,});
					mesh.Indices.push({rindex + 1, rindex + RadiusSlices, rindex + RadiusSlices + 1});
				}

				uint ni = index + (RadiusSlices - 1);
				mesh.Indices.push({ ni, ni + RadiusSlices, index });
				mesh.Indices.push({index + RadiusSlices, index, ni + RadiusSlices });
			}

			return mesh;
		}
	
		static MeshData Icosahedon(int refinement){
			
			MeshData mesh;
			Array<Vector3>& vertices = mesh.Vertices;
			float t = (1.0f + sqrtf(5.0f)) / 2.0f;

			vertices.push(Vector3(-1, t, 0));
			vertices.push(Vector3(1, t, 0));
			vertices.push(Vector3(-1, -t, 0));
			vertices.push(Vector3(1, -t, 0));
						 
			vertices.push(Vector3(0, -1, t));
			vertices.push(Vector3(0, 1, t));
			vertices.push(Vector3(0, -1, -t));
			vertices.push(Vector3(0, 1, -t));
						  
			vertices.push(Vector3(t, 0, -1));
			vertices.push(Vector3(t, 0, 1));
			vertices.push(Vector3(-t, 0, -1));
			vertices.push(Vector3(-t, 0, 1));

			Array<uint>& indices = mesh.Indices;

			indices.push({ 0, 11, 5 });
			indices.push({ 0, 5, 1 });
			indices.push({ 0, 1, 7 });
			indices.push({ 0, 7, 10 });
			indices.push({ 0, 10, 11 });

			// 5 adjacent faces
			indices.push({ 1, 5, 9 });
			indices.push({ 5, 11, 4 });
			indices.push({ 11, 10, 2 });
			indices.push({ 10, 7, 6 });
			indices.push({ 7, 1, 8 });

			// 5 faces around point 3
			indices.push({ 3, 9, 4 });
			indices.push({ 3, 4, 2 });
			indices.push({ 3, 2, 6 });
			indices.push({ 3, 6, 8 });
			indices.push({ 3, 8, 9 });

			// 5 adjacent faces
			indices.push({ 4, 9, 5 });
			indices.push({ 2, 4, 11 });
			indices.push({ 6, 2, 10 });
			indices.push({ 8, 6, 7 });
			indices.push({ 9, 8, 1 });


			return mesh;

		}
} raw;

	static Mesh Plane(uint faces) {
		auto data = raw.Plane(faces);
		Mesh m = Mesh();
		m.SetVertices(data.Vertices);
		m.SetIndices(data.Indices);
		m.SetNormals(data.Normals);
		m.SetUVs(data.UVs);
		return m;
	}

	static Mesh Cylinder(int slices){
        auto data = raw.Cylinder(slices);
		Mesh m = Mesh();
		m.SetVertices(data.Vertices);
		m.SetIndices(data.Indices);
		m.SetNormals(data.Normals);
		m.SetUVs(data.UVs);
		return m;
    }

    static Mesh Cube(){
		auto data = raw.Cube();
		Mesh m = Mesh();
		m.SetVertices(data.Vertices);
		m.SetIndices(data.Indices);
		m.SetNormals(data.Normals);
		m.SetUVs(data.UVs);
		return m;
    }

	static Mesh Sphere(int segments, int rings){
		MeshData data = raw.Sphere(segments, rings);
		Mesh m = Mesh();
		m.SetVertices(data.Vertices);
		m.SetIndices(data.Indices);
		m.SetNormals(data.Normals);
		m.SetUVs(data.UVs);
		return m;
	}
	
	static Mesh Icosahedron(int faces){
		MeshData data = raw.Icosahedon(faces);
		Mesh m = Mesh();
		m.SetVertices(data.Vertices);
		m.SetIndices(data.Indices);
		m.SetNormals(data.Normals);
		m.SetUVs(data.UVs);
		m.RecalculateNormals();
		return m;
	}
};