#pragma once
#include "atyp_Vector2.h"

static struct Window{
public:
	static int Width;
	static int Height; 

	static struct{
		int x = -1;
		int y = -1;
	} Position;

	static float aspectRatio;
};
