#pragma once
#include <atyp_Vector3.h>

class Mesh{
	int ItemCount;

	Vector3* Verticies;
	Vector3* Normals;
	Vector3* Colors;
	Vector2* UVs;

	int* indicies;
	int indexLength;
};