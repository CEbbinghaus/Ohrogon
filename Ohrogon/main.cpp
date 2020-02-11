#include <gl_core_4_5.h>
#include <glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <ext.hpp>
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

using uint = unsigned int;


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

	//Transform t;

	plane.transform.Scale = Vector3::one() * 10;
	plane.transform.Position = Vector3(0.0f, 0, -.5f);

	//t.Position.z = -1.0f;


	////t.Rotation = Vector3(0.f, 0.f, 0.f);
	//cam.getProjectionMatrix().Print();

	//cam.getViewMatrix().Print();

	//t.localTransform.Print();

	/*(cam.getPVMatrix() * t.localTransform).Print();*/

	float f = 0.0f;
	Vector2 camRotation(0.0f, 0.0f);

	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();

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

		if(f > 0.5f)
			camRotation += Mouse.GetDelta() * 0.005f;
		

		plane.transform.Rotation = Quaternion::euler(0.0f, f, 0.0f);

		//Quaternion CameraSpace =  * cam.transform.Rotation;

		//auto rotation = Matrix4();
		//// Left / Right rotation
		//rotation = rotation * Quaternion::aroundAngle(cam.transform.up(), float(-camRotation.x));
		//// Up / Down rotation
		//rotation = rotation * Quaternion::aroundAngle(Vector3::right(), float(-camRotation.x));

		cam.transform.Rotation;

		//cam.transform.Rotation.Print();
		
		//camRotation.y = Math::clamp(camRotation.y, -1.7f, 1.7f);
		Quaternion XRotation = Quaternion::euler(0.0f, camRotation.x, 0.0f);// *YRot;
		//Quaternion YRotation = Quaternion::aroundAngle(XRotation.toMatrix().ZAxis, camRotation.y);
		cam.transform.Rotation = XRotation;// *YRotation;

		//camRotation.y = Math::clamp(camRotation.y, 0.0f, 90.0f);
		
		Matrix4 t = XRotation.toMatrix();

		Vector3 forward = t.ZAxis;
		Vector3 right = t.XAxis;
		Vector3 up = Vector3::up();//cam.transform.up();

		/*Quaternion YRot = Quaternion::euler(0, camRotation.x, 0);
		Quaternion XRot = Quaternion::aroundAngle(YRot.toMatrix().XAxis, camRotation.y);*/

		//XRot.Print();



		//camRotation.Print();

		//printf("%f\n", forward.magnitude());

		glm::inverse(glm::mat4(1.0f));


		if (Keyboard['w'])
			cam.transform.Position += forward * 0.1;

		if (Keyboard['s'])
			cam.transform.Position += -forward * 0.1;

		if (Keyboard['d'])
			cam.transform.Position += -right * 0.1;

		if (Keyboard['a'])
			cam.transform.Position += right * 0.1;

		if (Keyboard[' '])
			cam.transform.Position += up * 0.1;

		if (Keyboard[KeyCode::LEFT_CONTROL])
			cam.transform.Position += -up * 0.1;


		cam.transform.Position.Print();
		//cam.transform.Position.Print();

		//Mouse.GetDelta().Print();

		//t.Position.x = sin(f) * 2.0f;

		//t.Rotation = Vector3(0.f, f, 0.f);

		//Matrix4 ModelTransform = t.updateTransform();

		Matrix4 pv_M = cam.getPVMatrix();
		//Matrix4 mvp = (pv_M * ModelTransform);

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