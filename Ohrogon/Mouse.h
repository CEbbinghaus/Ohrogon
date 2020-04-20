#pragma once
#include <atyp_Vector2.h>
#include <glfw3.h>
#include <cstdio>

#ifndef __Mouse

class MouseCallbackHelper;

class Mouse{
	
	static GLFWwindow* window;

	static Vector2 pos;
	static Vector2 delta;

	static void mouseCallback(GLFWwindow* window, double xpos, double ypos)
	{	
		delta.x = pos.x - (float)xpos;
		delta.y = pos.y - (float)ypos;

		pos.x = (float)xpos;
		pos.y = (float)ypos;
	}

public:

	static void Update() {
		if(window == nullptr)return;
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		Mouse::mouseCallback(window, x, y);
	}

	static void RegisterMouse(GLFWwindow* glWindow){
		window = glWindow;

		glfwSetCursorPosCallback(window, &Mouse::mouseCallback);

	}
	static Vector2 GetPosition() {
		return pos;
	}

	static Vector2 GetDelta() {
		return delta;
	}
};

GLFWwindow* Mouse::window = nullptr;
Vector2 Mouse::pos = Vector2::zero();
Vector2 Mouse::delta = Vector2::zero();
#endif
#define __Mouse