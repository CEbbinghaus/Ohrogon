#include <gl_core_4_5.h>
#include <glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <ext.hpp>
#include <chrono>
//#include <atyp_All.h>
#include <atyp_Transform.h>
#include <atyp_Math.h>
#include <atyp_Array.h>

#include "Shader.h"
#include "Camera.h"
#include "Window.h"
#include "KeyManager.h"
#include "MouseManager.h"
#include "Mesh.h"
#include "Primitive.h"
#include "Time.h"

using uint = unsigned int;
using Clock = std::chrono::steady_clock;

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (glfwInit() == false)
		return -1;

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Ohrogon Engine", nullptr, nullptr);

	if (window == nullptr) {
		glfwTerminate(); return -2;
	}

	glfwMakeContextCurrent(window);
	
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("Running OpenGL Version %i.%i\n", major, minor);

	//turn VSync off
	//glfwSwapInterval(0);

	//Key Manager Initialization
	KeyManager Keyboard = KeyManager::CreateKeyManager(window);

	//Mouse Configuration Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	//Mouse Initialization
	MouseManager Mouse = MouseManager::CreateMouseManager(window);

	

	//{
	//	int x, y;
	//	int width, height;
	//	glfwGetWindowSize(window, &width, &height);
	//	glfwGetWindowPos(window, &x, &y);
	//	Window::Position.x = x;
	//	Window::Position.y = y;
	//	Window::Width = width;
	//	Window::Height = height;
	//	Window::aspectRatio = ((float)width / height);
	//}

	Shader shader("./Shaders/VertShader.glsl", "./Shaders/FragShader.glsl");

	//Array<Vector3> verts = generatePlane(lod);
	//Array<uint> indxs = triangulatePlane(verts, lod);

	Mesh plane = Primitive::Plane(20);
	Mesh cylinder = Primitive::Cylinder(.5f, 1.0f, 6);

	Clock clock = Clock();
	
	//Vector3 vertices[] = {
	//	Vector3(0.5f,  0.5f, -0.5f),  // front top right
	//	Vector3(0.5f, -0.5f, -0.5f),  // front bottom right
	//	Vector3(-0.5f, -0.5f, -0.5f),  // front bottom left
	//	Vector3(-0.5f,  0.5f, -0.5f),  // front top left 

	//	Vector3( 0.5f,  0.5f, 0.5f),  // back top right
	//	Vector3( 0.5f, -0.5f, 0.5f),  // back bottom right
	//	Vector3(-0.5f, -0.5f, 0.5f),  // back bottom left
	//	Vector3(-0.5f,  0.5f, 0.5f)   // back top left 
	//};
	//uint indices[] = {
	//	0, 1, 3,   // front first triangle
	//	1, 2, 3,   // front second triangle

	//	//4, 5, 7,   // back first triangle
	//	//5, 6, 7,    // back second triangle
	//	//
	//	//4, 5, 1,   // right first triangle
	//	//1, 0, 4,    // right second triangle

	//	//3, 2, 6,   // left first triangle
	//	//6, 7, 3,    // left second triangle

	//	//0, 3, 7,   // top first triangle
	//	//7, 4, 0,    // top second triangle

	//	//1, 2, 6,   // bottom first triangle
	//	//6, 5, 1    // bottom second triangle
	//};

	glUseProgram(shader.ProgrammID);

	glClearColor(0.30f, 0.30f, 0.40f, 1);

	auto MVPMatrixUniform = glGetUniformLocation(shader.ProgrammID, "MVPMatrix");
	//auto ModelMatrixUniform = glGetUniformLocation(shader.ProgrammID, "ModelMatrix");

	Camera cam;

	cam.fov = 1.207f;
	cam.aspectRatio = 16 / 9.0f;
	cam.NearPlane = 0.01f;
	cam.FarPlane = 100000.0f;

	plane.transform.Scale = Vector3::one() * 10;
	plane.transform.Position = Vector3(-.5f, 0, -.5f);

	float f = 0.0f;
	Vector2 camRotation(0.0f, 0.0f);



	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();

		Time::Update();


		//{
		//	int x, y;
		//	int width, height;
		//	glfwGetWindowSize(window, &width, &height);
		//	glfwGetWindowPos(window, &x, &y);
		//	Window::Position.x = x;
		//	Window::Position.y = y;
		//	Window::Width = width;
		//	Window::Height = height;
		//	Window::aspectRatio = ((float)width / height);
		//}

		f += 0.01f;

		Vector2 offset;

		if (f > 0.5f)
			camRotation += (Mouse.GetDelta() * 0.005f) * Time::OneTime;

		
		camRotation.y = Math::clamp(camRotation.y, -1.2f,1.2f);

		cam.transform.Rotation = Quaternion::euler(camRotation.y, camRotation.x, 0.0f);


		Vector3 forward = cam.transform.forward();
		Vector3 right = cam.transform.right();
		Vector3 up = Vector3::up();
		
		Vector3 movement;

		if (Keyboard['w'])
			movement += forward * 0.1;

		if (Keyboard['s'])
			movement += -forward * 0.1;

		if (Keyboard['d'])
			movement += -right * 0.1;

		if (Keyboard['a'])
			movement += right * 0.1;

		if (Keyboard[' '])
			movement += up * 0.1;

		if (Keyboard[KeyCode::LEFT_CONTROL])
			movement += -up * 0.1;

		cam.transform.Position += movement * Time::OneTime;

		Matrix4 pv_M = cam.getPVMatrix();

		plane.draw(MVPMatrixUniform, pv_M);

		cylinder.draw(MVPMatrixUniform, pv_M);

		glfwSwapBuffers(window);
		
		MouseCallbackHelper::Callback(window);
	}

	Keyboard.DestroyKeyManager();
	Mouse.DestroyMouseManager();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}