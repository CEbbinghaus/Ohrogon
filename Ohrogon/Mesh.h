#pragma once
#include <atyp_Vector3.h>
#include <atyp_Transform.h>
#include <atyp_Array.h>
#include <gl_core_4_5.h>

using uint = unsigned int;


//TODO: Optimise the VRam Buffer Allocation and Rewriting
class Mesh {
public:

	uint VAO;
	uint VBO;
	uint IBO;

	Array<Vector3> Verticies;
	Array<Vector3> Normals;
	Array<Vector3> Colors;
	Array<Vector2> UVs;

	Array<uint> Indices;

	Transform transform;

	Mesh() {

		GenerateBuffers();

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, Verticies.length * sizeof(Vector3), Verticies.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.length * sizeof(uint), Indices.data(), GL_STATIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
	}

	Mesh(const Array<Vector3>& verts, const Array<uint>& indxs) : Verticies(verts), Indices(indxs) {
		
		GenerateBuffers();
		
		BindVerticies();

		BindIndices();

		UpdateVertexAttributes();
	}

	Mesh(const Mesh& original){
		GenerateBuffers();

		Verticies = original.Verticies;
		Normals = original.Normals;
		Colors = original.Colors;
		UVs = original.UVs;

		BindUVs();

		UpdateVertexAttributes();
	}

	Mesh(Mesh&& original) noexcept{
		GenerateBuffers();

		Verticies = original.Verticies;
		Normals = original.Normals;
		Colors = original.Colors;
		UVs = original.UVs;

		BindUVs();

		UpdateVertexAttributes();
	}

	~Mesh() {
		DeleteBuffers();
	}

	void GenerateBuffers(){
		glGenVertexArrays(1, &VAO);

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &IBO);
	}

	void DeleteBuffers(){
		glDeleteBuffers(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &IBO);
	}

	void UpdateVertexAttributes() {
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glDisableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glDisableVertexAttribArray(1);
		if (Normals.length) {
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(Verticies.length * sizeof(Vector3)));
			glEnableVertexAttribArray(1);
		}

		glDisableVertexAttribArray(2);
		if (UVs.length) {
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)((Verticies.length + Normals.length) * sizeof(Vector3)));
			glEnableVertexAttribArray(2);
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	void AllocateBuffer() {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, (Verticies.length + (long)Normals.length) * sizeof(Vector3) + (UVs.length * sizeof(Vector2)), 0, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void BindIndices() {
		glBindVertexArray(VAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.length * sizeof(uint), Indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	void BindVerticies() {
		AllocateBuffer();
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, Verticies.length * sizeof(Vector3), Verticies.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void BindNormals() {
		BindVerticies();
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, Verticies.length * sizeof(Vector3), Normals.length * sizeof(Vector3), Normals.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void BindUVs() {
		BindNormals();
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, (Verticies.length + Normals.length) * sizeof(Vector3), UVs.length * sizeof(Vector2), Normals.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SetIndices(Array<uint> indxs) {
		Indices = indxs;

		BindIndices();
	}

	void SetVerticies(Array<Vector3> verts) {
		glBindVertexArray(VAO);
		Verticies = verts;
		BindVerticies();
		glBindVertexArray(0);
	}

	void SetNormals(Array<Vector3> normals){

		if (normals.length && normals.length != Verticies.length)
			throw "Normals and Verticies Must have the same length";

		Normals = normals;

		glBindVertexArray(VAO);
		BindNormals();
		UpdateVertexAttributes();
		glBindVertexArray(0);
	}

	void SetUVs(Array<Vector2> uvs){

	}

	void FlatShade() {
		Array<Vector3> oldVerts = Verticies;
		Array<uint> oldIndicies = Indices;

		Array<Vector3> newVerts;
		Array<uint> newIndicies;

		int i = 0;
		for (uint& index : oldIndicies){
			newVerts.push(oldVerts[index]);
			newIndicies.push(newVerts.length - 1);
		}

		SetVerticies(newVerts);
		SetIndices(newIndicies);
		RecalculateNormals();
	}

	void SmoothShade(){
		Array<Vector3> oldVerts = Verticies;
		Array<uint> oldIndicies = Indices;

		Array<Vector3> newVerts;
		Array<uint> newIndicies;

		int i = 0;
		for (uint& index : oldIndicies) {
			Vector3 b = oldVerts[index];
			index = newVerts.indexOf(b);
			if (index == -1) {
				newVerts.push(b);
				newIndicies.push(newVerts.length - 1);
			}
			else
				newIndicies.push(index);
		}

		SetVerticies(newVerts);
		SetIndices(newIndicies);
		RecalculateNormals();
	}

	void draw(uint MVPMatrixUniform, Matrix4 ProjectionView) {
		glBindVertexArray(VAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glUniformMatrix4fv(MVPMatrixUniform, 1, false, ProjectionView * transform.updateTransform());
		//glUniformMatrix4fv(ModelMatrixUniform, 1, false, t.localTransform);

		/*glPolygonMode(GL_FRONT, GL_FILL);

		glDrawElements(GL_TRIANGLES, Indices.length, GL_UNSIGNED_INT, 0);


		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/

		glDrawElements(GL_TRIANGLES, Indices.length, GL_UNSIGNED_INT, 0);


		glBindVertexArray(0);
	}

	void RecalculateNormals(){
		Array<Vector3> normals(Verticies.length);

		for (int i = 0; i < normals.length; ++i) {
			normals[i] = Vector3();
		}

		/*normals.map([](Vector3 vector, int index){
			return Vector3(0, 0, 0);
		});*/

		for(int i = 0; i < Indices.length; i += 3){
			int a = Indices[i];
			int b = Indices[i + 1];
			int c = Indices[i + 2];

			Vector3 Normal = Vector3::FaceNormal(Verticies[a], Verticies[b], Verticies[c]);

			normals[a] += Normal;
			normals[b] += Normal;
			normals[c] += Normal;
		}


		for (int i = 0; i < normals.length; ++i) {
			normals[i].normalise();
		}
	/*	normals.forEach([](Vector3 Element, int index){
			Element.normalise();
		});*/

		SetNormals(normals);
	}

	Mesh& operator=(const Mesh& original){
		Verticies = original.Verticies;
		Normals = original.Normals;
		Colors = original.Colors;
		UVs = original.UVs;

		BindUVs();
		return *this;
	}
};