#pragma once
#include "glfw3.h"
#include "atyp_Vector2.h"

class Window{
	static Window* Instance;

	Window(){
	}
public:

	Window(const Window&) = delete;
	Window operator=(const Window&) = delete;

	static GLFWwindow* ptr;

	union{
		struct{
			float Width;
			float Height;
		};
		Vector2 size;
	};

	static struct{
		int x = -1;
		int y = -1;
	} Position;

	static float aspectRatio;

	static Window& Create(GLFWwindow* glwindow){
		static Window* window = new Window();
		window->ptr = glwindow;
		Instance = window;
		return *window;
	}

	static void Destroy(){
		delete Instance;
		Instance = nullptr;
	}

	static void Update(){
		int x, y;
		int width, height;
		glfwGetWindowPos(ptr, &x, &y);
		glfwGetWindowSize(ptr, &width, &height);
	}

	
};

float Window::aspectRatio = 1.0f;