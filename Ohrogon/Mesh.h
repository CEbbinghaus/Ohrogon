#pragma once
#include <atyp_Vector3.h>
#include <atyp_Transform.h>
#include <atyp_Array.h>
#include <gl_core_4_5.h>

using uint = unsigned int;

struct MeshData{
	Array<Vector3> Vertices;
	Array<Vector3> Normals;
	Array<Vector3> Colors;
	Array<Vector3> Tangents;
	Array<Vector3> BiTangents;
	Array<Vector2> UVs;

	Array<uint> Indices;
};

//TODO: Optimise the VRam Buffer Allocation and Rewriting
class Mesh: public MeshData {
public:

	uint VAO;
	uint VBO;
	uint IBO;

	Transform transform;

	Mesh() {

		GenerateBuffers();

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, Vertices.length * sizeof(Vector3), Vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.length * sizeof(uint), Indices.data(), GL_STATIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
	}

	Mesh(const Array<Vector3>& verts, const Array<uint>& indxs)
	{
		Vertices = verts;
		Indices = indxs;
		
		GenerateBuffers();
		
		BindVertices();

		BindIndices();

		UpdateVertexAttributes();
	}

	Mesh(const Mesh& original) {
		printf("Copy Constructor Called");
		GenerateBuffers();
		SetVertices(original.Vertices);
		SetIndices(original.Indices);
		SetNormals(original.Normals);
		SetUVs(original.UVs);
		UpdateVertexAttributes();
	}

	//Mesh(const Mesh& original){
	//	GenerateBuffers();

	//	Vertices = original.Vertices;
	//	Normals = original.Normals;
	//	Colors = original.Colors;
	//	UVs = original.UVs;

	//	BindUVs();

	//	UpdateVertexAttributes();
	//}

	//Mesh(Mesh&& original) noexcept{
	//	GenerateBuffers();

	//	Vertices = original.Vertices;
	//	Normals = original.Normals;
	//	Colors = original.Colors;
	//	UVs = original.UVs;

	//	BindUVs();

	//	UpdateVertexAttributes();
	//}

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

		uint length = Vertices.length * sizeof(Vector3);

		glDisableVertexAttribArray(1);
		if (Normals.length) {
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(length));
			glEnableVertexAttribArray(1);

			length += Normals.length * sizeof(Vector3);
		}

		glDisableVertexAttribArray(2);
		if (UVs.length) {
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(length));
			glEnableVertexAttribArray(2);
			
			length += UVs.length * sizeof(Vector2);
		}

		if(Tangents.length){
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(length));
			glEnableVertexAttribArray(2);

			length += Tangents.length * sizeof(Vector3);
		}

		if(BiTangents.length){
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)(length));
			glEnableVertexAttribArray(2);

			length += BiTangents.length * sizeof(Vector3);
		}
		
 		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	void AllocateBuffer() {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, (Vertices.length + (long)Normals.length) * sizeof(Vector3) + (UVs.length * sizeof(Vector2)), 0, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void BindIndices() {
		glBindVertexArray(VAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.length * sizeof(uint), Indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	void BindVertices() {
		AllocateBuffer();
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, Vertices.length * sizeof(Vector3), Vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void BindNormals() {
		BindVertices();
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, Vertices.length * sizeof(Vector3), Normals.length * sizeof(Vector3), Normals.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void BindUVs() {
		BindNormals();
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, (Vertices.length + Normals.length) * sizeof(Vector3), UVs.length * sizeof(Vector2), UVs.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void BindTangents(){
		BindUVs();
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, (Vertices.length + Normals.length) * sizeof(Vector3) + (UVs.length) * sizeof(Vector2), Tangents.length * sizeof(Vector3), Tangents.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void BindBiTangents(){
		BindTangents();
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, (Vertices.length + Normals.length + Tangents.length) * sizeof(Vector3) + (UVs.length) * sizeof(Vector2), BiTangents.length * sizeof(Vector3), BiTangents.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SetIndices(Array<uint> indxs) {
		glBindVertexArray(VAO);
		Indices = indxs;
		BindIndices();
		glBindVertexArray(0);
	}

	void SetVertices(Array<Vector3> verts) {
		glBindVertexArray(VAO);
		Vertices = verts;
		BindVertices();
		glBindVertexArray(0);
	}

	void SetNormals(Array<Vector3> normals){
		//if (!normals.length)return;

		Normals = normals;

		glBindVertexArray(VAO);
		BindNormals();
		UpdateVertexAttributes();
		glBindVertexArray(0);
	}

	void SetUVs(Array<Vector2> uvs){
		//if (!uvs.length) return;

		UVs = uvs;

		glBindVertexArray(VAO);
		BindUVs();
		UpdateVertexAttributes();
		glBindVertexArray(0);
	}

	void SetTangents(const Array<Vector3>& tangents){
		Tangents = tangents;

		glBindVertexArray(VAO);
		BindTangents();
		UpdateVertexAttributes();
		glBindVertexArray(0);
	}

	void SetBiTangents(const Array<Vector3>& bitangents){
		BiTangents = bitangents;

		glBindVertexArray(VAO);
		BindBiTangents();
		UpdateVertexAttributes();
		glBindVertexArray(0);
	}

	void FlatShade() {
		Array<Vector3> oldVerts = Vertices;
		Array<uint> oldIndicies = Indices;
		Array<Vector2> oldUVs = UVs;
		
		Array<Vector3> newVerts;
		Array<uint> newIndicies;
		Array<Vector2> newUVs;

		int i = 0;
		for (uint& index : oldIndicies){
			newVerts.push(oldVerts[index]);

			newUVs.push(oldUVs[index]);

			newIndicies.push(newVerts.length - 1);
		}
		
		SetUVs(newUVs);
		SetVertices(newVerts);
		SetIndices(newIndicies);
		RecalculateNormals();
	}

	void SmoothShade(){
		Array<Vector3> oldVerts = Vertices;
		Array<uint> oldIndicies = Indices;
		Array<Vector2> oldUVs = UVs;

		Array<Vector3> newVerts;
		Array<uint> newIndicies;
		Array<Vector2> newUVs;

		int i = 0;
		for (uint& index : oldIndicies) {
			Vector3 b = oldVerts[index];
			int indx = newVerts.indexOf(b);
			if (indx == -1) {
				newVerts.push(b);

				newUVs.push(oldUVs[index]);

				newIndicies.push(newVerts.length - 1);
			}
			else
				newIndicies.push(indx);
		}

		SetVertices(newVerts);
		SetIndices(newIndicies);
		SetUVs(newUVs);
		RecalculateNormals();
	}

	void draw(uint MVPMatrixUniform, Matrix4 ProjectionView) {
		glBindVertexArray(VAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		//glUniformMatrix4fv(MVPMatrixUniform, 1, false, ProjectionView * transform.updateTransform());
		//glUniformMatrix4fv(ModelMatrixUniform, 1, false, t.localTransform);

		/*glPolygonMode(GL_FRONT, GL_FILL);

		glDrawElements(GL_TRIANGLES, Indices.length, GL_UNSIGNED_INT, 0);


		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/

		glDrawElements(GL_TRIANGLES, Indices.length, GL_UNSIGNED_INT, 0);


		glBindVertexArray(0);
	}

	void RecalculateNormals(){
		Array<Vector3> normals(Vertices.length);

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

			Vector3 Normal = Vector3::FaceNormal(Vertices[a], Vertices[b], Vertices[c]);

			normals[a] += Normal;
			normals[b] += Normal;
			normals[c] += Normal;
		}


		/*for (int i = 0; i < normals.length; ++i) {
			normals[i].normalise();
		}*/

		normals.forEach([](Vector3& Element){
			Element.normalise();
		});

		SetNormals(normals);
	}

	void CalculateTangents(){
		if(UVs.isEmpty())return;

		Array<Vector3> TangentBuffer;
		Array<Vector3> BiTangentBuffer;

		for(int i = 0; i < Indices.length - 3; i += 3){
			Vector3 PositionDelta1 = Vertices[i + 1] - Vertices[i];
			Vector3 PositionDelta2 = Vertices[i + 2] - Vertices[i];

			Vector2 UVDelta1 = UVs[i + 1] - UVs[i];
			Vector2 UVDelta2 = UVs[i + 2] - UVs[i];

			float r = 1.0f / (UVDelta1.x * UVDelta2.y - UVDelta1.y * UVDelta2.x);
			
			Vector3 Tangent = (PositionDelta1 * UVDelta2.y - PositionDelta2 * UVDelta1.y) * r;
			Vector3 BiTangent = (PositionDelta2 * UVDelta1.x - PositionDelta1 * UVDelta2.x) * r;

			TangentBuffer.push({Tangent, Tangent, Tangent});
			BiTangentBuffer.push({BiTangent, BiTangent, BiTangent});
		}

		SetTangents(TangentBuffer);
		SetBiTangents(BiTangentBuffer);
	}

	//Mesh& operator=(const Mesh& original){
	//	Vertices = original.Vertices;
	//	Normals = original.Normals;
	//	Colors = original.Colors;
	//	UVs = original.UVs;

	//	BindUVs();
	//	return *this;
	//}
};