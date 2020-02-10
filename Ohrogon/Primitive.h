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
				verts[x * (faces + 1) + y] = Vector3(x / (float)faces, 0.0f, y / (float)faces);
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

	static Mesh Cylinder(float radius, float halfLength, int slices)
    {
        Array<Vector3> vertices;
        Array<uint> indices;

        vertices.push(Vector3(0.0f, halfLength, 0.0f));
        vertices.push(Vector3(0.0f, -halfLength, 0.0f));

        for (int i = 0; i < slices; i++)
        {
            float theta = Math::TWO_PI() * ((float)i / slices);

            vertices.push(Vector3(radius * glm::cos(theta), halfLength, radius * glm::sin(theta)));
            vertices.push(Vector3(radius * glm::cos(theta), -halfLength, radius * glm::sin(theta)));
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
};