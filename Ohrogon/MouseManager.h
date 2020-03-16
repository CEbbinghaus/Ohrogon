#pragma once
#include <atyp_Vector2.h>
#include <glfw3.h>
#include <cstdio>

class MouseCallbackHelper;

class MouseManager
{
	friend MouseCallbackHelper;

	static MouseManager* Instance;
	
	GLFWwindow* window;

	Vector2* pos;
	Vector2* delta;

	static void mouseCallback(GLFWwindow* window, double xpos, double ypos)
	{	
		if (MouseManager::Instance == nullptr)return;

		Instance->delta->x = Instance->pos->x - (float)xpos;
		Instance->delta->y = Instance->pos->y - (float)ypos;

		Instance->pos->x = (float)xpos;
		Instance->pos->y = (float)ypos;
	}

public:

	static MouseManager& CreateMouseManager(GLFWwindow* window) {
		if (Instance != nullptr)return *Instance;

		Instance = new MouseManager();

		Instance->pos = new Vector2();
		Instance->delta = new Vector2();

		Instance->window = window;

		glfwSetCursorPosCallback(window, &MouseManager::mouseCallback);

		return *Instance;
	}

	static void DestroyMouseManager() {
		delete Instance->pos;
		delete Instance->delta;
		delete Instance;
		Instance = nullptr;
	}

	Vector2 GetPosition() {
		return *pos;
	}

	Vector2 GetDelta() {
		return *delta;
	}
};


class MouseCallbackHelper {
public:
	static void Callback(GLFWwindow* window) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		MouseManager::mouseCallback(window, x, y);
	}
};
