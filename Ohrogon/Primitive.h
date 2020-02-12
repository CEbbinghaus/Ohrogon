#pragma once

#include <atyp_Array.h>
#include <atyp_Vector3.h>
#include <atyp_Math.h>
#include "Mesh.h"

static class Primitive {
public:

	static Mesh Plane(uint faces) {
		Array verts = Array<Vector3>((faces + 1) * (faces + 1));

		for (int x = 0; x <= faces; ++x) {
			for (int y = 0; y <= faces; ++y) {
				verts[x * (faces + 1) + y] = Vector3((x / (float)faces) - 0.5f, 0.0f, (y / (float)faces) - 0.5f);
			}
		}


		uint indexSize = faces * faces * 6;
		Array<uint> indices = Array<uint>(indexSize);

		int index = 0;
		for (int x = 0; x < faces; ++x) {
			for (int y = 0; y < faces; ++y) {
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

		return Mesh(verts, indices);
	}

	static Mesh Cylinder(int slices)
    {
        Array<Vector3> vertices;
        Array<uint> indices;

        vertices.push(Vector3(0.0f, 0.5f, 0.0f));
        vertices.push(Vector3(0.0f, -0.5f, 0.0f));

        for (int i = 0; i < slices; i++)
        {
            float theta = Math::TWO_PI() * ((float)i / slices);

            vertices.push(Vector3(cos(theta) * 0.5f, 0.5f, sin(theta) * 0.5f));
            vertices.push(Vector3(cos(theta) * 0.5f, -0.5f, sin(theta) * 0.5f));
        }

        for (int i = 2; i < vertices.size() - 2; i += 2)
        {
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

        return Mesh(vertices, indices);
    }

    static Mesh Cube(){
		Array<Vector3> vertices = {
			Vector3(0.5f,  0.5f, -0.5f),  // front top right
			Vector3(0.5f, -0.5f, -0.5f),  // front bottom right
			Vector3(-0.5f, -0.5f, -0.5f),  // front bottom left
			Vector3(-0.5f,  0.5f, -0.5f),  // front top left 

			Vector3( 0.5f,  0.5f, 0.5f),  // back top right
			Vector3( 0.5f, -0.5f, 0.5f),  // back bottom right
			Vector3(-0.5f, -0.5f, 0.5f),  // back bottom left
			Vector3(-0.5f,  0.5f, 0.5f)   // back top left 
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
		return Mesh(vertices, indices);
    }
};