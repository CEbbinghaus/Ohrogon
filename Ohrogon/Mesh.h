#pragma once
#include <atyp_Vector3.h>
#include <atyp_Transform.h>
#include <atyp_Array.h>
#include <gl_core_4_5.h>

using uint = unsigned int;

class Mesh{
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

		glGenVertexArrays(1, &VAO);

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &IBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, Verticies.length * sizeof(Vector3), Verticies.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.length * sizeof(uint), Indices.data(), GL_STATIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
	}

	Mesh(Array<Vector3> verts, Array<uint> indxs): Verticies(verts), Indices(indxs) {

		glGenVertexArrays(1, &VAO);

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &IBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, Verticies.length * sizeof(Vector3), Verticies.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.length * sizeof(uint), Indices.data(), GL_STATIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
	}

	~Mesh() {
		glDeleteBuffers(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &IBO);
	}

	void SetVerticies(Array<Vector3> verts) {
		Verticies = verts;
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, Verticies.length * sizeof(Vector3), Verticies.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	void SetIndices(Array<uint> indxs) {
		Indices = indxs;
		glBindVertexArray(VAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.length * sizeof(uint), Indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
		glBindVertexArray(0);
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

		normals.map([](Vector3 vector, int index){
			return Vector3();
		});

		for(int i = 0; i < Indices.length; i += 3){
			int a = Indices[i];
			int b = Indices[i + 1];
			int c = Indices[i + 2];

			Vector3 Normal = Vector3::FaceNormal(Verticies[a], Verticies[b], Verticies[c]);

			normals[a] += Normal;
			normals[b] += Normal;
			normals[c] += Normal;
		}

		normals.forEach([](Vector3 Element, int index){
			Element.normalise();
		});

		Normals = normals;
	}
};