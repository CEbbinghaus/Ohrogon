#pragma once
#include <atyp_Array.h>
#include <atyp_Transform.h>
#include <atyp_Vector3.h>
#include <gl_core_4_5.h>
#include "Component.h"


using uint = unsigned int;

struct MeshData {
    Array<Vector3> Vertices;
    Array<Vector3> Normals;
    Array<Vector3> Colors;
    Array<Vector3> Tangents;
    Array<Vector3> BiTangents;
    Array<Vector2> UVs;

    Array<uint> Indices;
};

//TODO: Optimise the VRam Buffer Allocation and Rewriting
class Mesh : public MeshData, public Component {

   public:
    uint VAO;
    uint VBO;
    uint IBO;

    Transform transform;

    Mesh(){
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

    Mesh(const Array<Vector3>& verts, const Array<uint>& indxs) {
        Vertices = verts;
        Indices = indxs;

        GenerateBuffers();

        BindVertices();

        BindIndices();

        UpdateVertexAttributes();
    }

    Mesh(const Mesh& original) {
        GenerateBuffers();
        SetVertices(original.Vertices);
        SetIndices(original.Indices);
        SetNormals(original.Normals);
        SetUVs(original.UVs);
        UpdateVertexAttributes();
    }

    ~Mesh() {
        DeleteBuffers();
    }

    void GenerateBuffers() {
        glGenVertexArrays(1, &VAO);

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &IBO);
    }

    void DeleteBuffers() {
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
        if (UVs.length) {
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(length));
            glEnableVertexAttribArray(1);

            length += UVs.length * sizeof(Vector2);
        }

        glDisableVertexAttribArray(2);
        if (Normals.length) {
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(length));
            glEnableVertexAttribArray(2);

            length += Normals.length * sizeof(Vector3);
        }


        glDisableVertexAttribArray(3);
        if (Tangents.length) {
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(length));
            glEnableVertexAttribArray(3);

            length += Tangents.length * sizeof(Vector3);
        }

        glDisableVertexAttribArray(4);
        if (BiTangents.length) {
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)(length));
            glEnableVertexAttribArray(4);

            length += BiTangents.length * sizeof(Vector3);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    void AllocateBuffer() {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, (Vertices.length + (long)Normals.length + Tangents.length + BiTangents.length) * sizeof(Vector3) + (UVs.length * sizeof(Vector2)), 0, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void BindIndices() {
        glBindVertexArray(VAO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.length * sizeof(uint), Indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    void BindAllData() {
        AllocateBuffer();
        BindVertices();
        BindNormals();
        BindUVs();
        BindTangents();
        BindBiTangents();
    }

    void BindVertices() {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, Vertices.length * sizeof(Vector3), Vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void BindNormals() {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, Vertices.length * sizeof(Vector3), Normals.length * sizeof(Vector3), Normals.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void BindUVs() {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, (Vertices.length + Normals.length) * sizeof(Vector3), UVs.length * sizeof(Vector2), UVs.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void BindTangents() {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, (Vertices.length + Normals.length) * sizeof(Vector3) + (UVs.length) * sizeof(Vector2), Tangents.length * sizeof(Vector3), Tangents.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void BindBiTangents() {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, (Vertices.length + Normals.length + Tangents.length) * sizeof(Vector3) + (UVs.length) * sizeof(Vector2), BiTangents.length * sizeof(Vector3), BiTangents.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void SetIndices(Array<uint> indxs) {
        Indices = indxs;

        glBindVertexArray(VAO);
        BindIndices();
        glBindVertexArray(0);
    }

    void SetVertices(Array<Vector3> verts) {
        Vertices = verts;

        glBindVertexArray(VAO);
        BindAllData();
        UpdateVertexAttributes();
        glBindVertexArray(0);
    }

    void SetNormals(Array<Vector3> normals) {
        //if (!normals.length)return;

        Normals = normals;

        glBindVertexArray(VAO);
        BindAllData();
        UpdateVertexAttributes();
        glBindVertexArray(0);
    }

    void SetUVs(Array<Vector2> uvs) {
        //if (!uvs.length) return;

        UVs = uvs;

        glBindVertexArray(VAO);
        BindAllData();
        UpdateVertexAttributes();
        glBindVertexArray(0);
    }

    void SetTangents(const Array<Vector3>& tangents) {
        Tangents = tangents;

        glBindVertexArray(VAO);
        BindAllData();
        UpdateVertexAttributes();
        glBindVertexArray(0);
    }

    void SetBiTangents(const Array<Vector3>& bitangents) {
        BiTangents = bitangents;

        glBindVertexArray(VAO);
        BindAllData();
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
        for (uint& index : oldIndicies) {
            newVerts.push(oldVerts[index]);

            newUVs.push(oldUVs[index]);

            newIndicies.push(newVerts.length - 1);
        }

        SetUVs(newUVs);
        SetVertices(newVerts);
        SetIndices(newIndicies);
        RecalculateNormals();
    }

    void SmoothShade() {
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
            } else
                newIndicies.push(indx);
        }

        SetVertices(newVerts);
        SetIndices(newIndicies);
        SetUVs(newUVs);
        RecalculateNormals();
    }

    void draw(uint MVPMatrixUniform, uint ModelMatrixUniform, Matrix4 ProjectionView) {
        glBindVertexArray(VAO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

        Matrix4 ModelMatrix = transform.updateTransform();
        glUniformMatrix4fv(MVPMatrixUniform, 1, false, ProjectionView * ModelMatrix);
        glUniformMatrix4fv(ModelMatrixUniform, 1, false, ModelMatrix);

        //glUniformMatrix4fv(MVPMatrixUniform, 1, false, ProjectionView * transform.updateTransform());
        //glUniformMatrix4fv(ModelMatrixUniform, 1, false, t.localTransform);

        /*glPolygonMode(GL_FRONT, GL_FILL);

		glDrawElements(GL_TRIANGLES, Indices.length, GL_UNSIGNED_INT, 0);


		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/

        glDrawElements(GL_TRIANGLES, Indices.length, GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    void RecalculateNormals() {
        Array<Vector3> normals(Vertices.length);

        for (int i = 0; i < normals.length; ++i) {
            normals[i] = Vector3();
        }

        /*normals.map([](Vector3 vector, int index){
			return Vector3(0, 0, 0);
		});*/

        for (int i = 0; i < Indices.length; i += 3) {
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

        normals.forEach([](Vector3& Element) {
            Element.normalise();
        });

        SetNormals(normals);
        if (!UVs.isEmpty())
            CalculateTangents();
    }

    void CalculateTangents() {
        if (UVs.isEmpty()) return;

        Array<Vector3> TangentBuffer;
        Array<Vector3> BiTangentBuffer;

        for (int i = 0; i < Indices.length; i += 3) {
            uint i0 = Indices[i];
            uint i1 = Indices[i + 1];
            uint i2 = Indices[i + 2];

            Vector3 FaceNormal = Vector3::FaceNormal(Vertices[i0], Vertices[i1], Vertices[i2]);

            Vector3 deltaPos1 = Vertices[i1] - Vertices[i0];
            Vector3 deltaPos2 = Vertices[i2] - Vertices[i0];

            Vector2 deltaUV1 = UVs[i1] - UVs[i0];
            Vector2 deltaUV2 = UVs[i2] - UVs[i0];

            // float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

            // Vector3 Tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
            // Vector3 BiTangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
            float res = (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
            Vector3 Tangent;
            Vector3 BiTangent;
            if (res != 0.0f) {
                float r = 1.0f / res;
                Tangent = ((deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r);
                BiTangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
            } else {
                Tangent = Vector3::right();//Vector3(FaceNormal.y, -FaceNormal.x, FaceNormal.z);
                BiTangent = Vector3::up();//Vector3::cross(Tangent, FaceNormal);
            }

            Tangent.normalise();
            BiTangent.normalise();

            TangentBuffer.push({Tangent, Tangent, Tangent});
            BiTangentBuffer.push({BiTangent, BiTangent, BiTangent});
        }

        // for (int i = 0; i < Vertices.length; i++)
        // {
        // 	Vector3& t = TangentBuffer[i];
        // 	Vector3& b = BiTangentBuffer[i];
        // 	Vector3& n = Normals[i];
        // 	TangentBuffer[i] = ((t - n) * Vector3::dot(t, n));
        // 	//Tangents[i].w = (Vector3::dot(Vector3::cross(t, b), n) > 0.0f) ? 1.0f : −1.0f;
        // }

        SetTangents(TangentBuffer);
        SetBiTangents(BiTangentBuffer);
    }

    Symbol _IDENTIFIER(){
        return Symbol("Component.Mesh");
    }
};