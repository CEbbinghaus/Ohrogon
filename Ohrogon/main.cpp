#include <gl_core_4_5.h>
#include <glfw3.h>
#include <iostream>
//#include <atyp_All.h>
#include "Shader.h"
#include "atyp_Transform.h"
#include "Camera.h"
#include "Window.h"
#include "KeyManager.h"
#include "MouseManager.h"
#include <atyp_Math.h>

using uint = unsigned int;

Vector3* generatePlane(int lod, int& vertCount){
	Vector3* points = new Vector3[(lod + 1) * (lod + 1)];
	for(int x = 0; x <= lod; ++x){
		for(int y = 0; y <= lod; ++y){
			points[x * (lod + 1) + y] = Vector3(x / (float)lod, 0.0f, y / (float)lod);
		}
	}
	vertCount = (lod + 1) * (lod + 1);
	return points;
}

uint* triangulatePlane(Vector3* verticies, int lod, int& indxCount){
	uint indexSize = lod * lod * 6;
	uint* indices = new uint[indexSize];
	memset(indices, -1, indexSize * sizeof(uint));
	indxCount = indexSize;
	int index = 0;
	for(int x = 0; x < lod; ++x){
		for(int y = 0; y < lod; ++y){
			int nextLine = (lod + 1);
			uint position = (x * nextLine + y);

			indices[index++] = position;
			indices[index++] = position + 1;
			indices[index++] = position + nextLine;

			indices[index++] = position + 1;
			indices[index++] = position + nextLine + 1;
			indices[index++] = position + nextLine;
		}
	}

	printf("%d\n", index);

	return indices;
}

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

	int lod = 50;

	int vertCount, indxCount;

	Vector3* verts = generatePlane(lod, vertCount);
	uint* indxs = triangulatePlane(verts, lod, indxCount);

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

	uint VAO;
	uint VBO;
	uint IBO;

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Vector3), verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indxCount * sizeof(uint), indxs, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glUseProgram(shader.ProgrammID);

	glClearColor(0.30f, 0.30f, 0.40f, 1);


	auto MVPMatrixUniform = glGetUniformLocation(shader.ProgrammID, "MVPMatrix");
	//auto ModelMatrixUniform = glGetUniformLocation(shader.ProgrammID, "ModelMatrix");

	Camera cam;

	cam.fov = 1.207f;
	cam.aspectRatio = 16 / 9.0f;
	cam.NearPlane = 0.01f;
	cam.FarPlane = 100000.0f;

	Transform t;

	t.Scale = Vector3::one() * 10;

	t.updateTransform();

	//t.Position.z = -1.0f;

	t.Position = Vector3(-.5f, 0, -.5f);

	////t.Rotation = Vector3(0.f, 0.f, 0.f);
	//cam.getProjectionMatrix().Print();

	//cam.getViewMatrix().Print();

	//t.localTransform.Print();

	/*(cam.getPVMatrix() * t.localTransform).Print();*/

	float f = 0.0f;
	//for (int i = 0; i < 6; ++i) {
	//	Vector4 scaledVert = mvp * Vector4(vertices[i], 1.0f);
	//	std::cout << scaledVert.x << " " << scaledVert.y << " " << scaledVert.z << std::endl;
	//}
	Vector2 camRotation(1.5708f, 0.0f);

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
		
		camRotation += Mouse.GetDelta() * 0.01f;

		//camRotation.y = Math::clamp(camRotation.y, 0.0f, 90.0f);

		Vector3 forward = cam.transform.forward();
		Vector3 right = cam.transform.right();
		Vector3 up = Vector3::up();//cam.transform.up();

		Quaternion YRot = Quaternion::euler(0, camRotation.x, 0);
		Quaternion XRot = Quaternion::aroundAngle(YRot.toMatrix().XAxis, camRotation.y);

		YRot.toMatrix().XAxis.Print();

		cam.transform.Rotation = XRot * YRot;


		//camRotation.Print();

		//printf("%f\n", forward.magnitude());



		if (Keyboard['w'])
			cam.transform.Position += forward * 0.1;

		if (Keyboard['s'])
			cam.transform.Position += -forward * 0.1;

		if (Keyboard['d'])
			cam.transform.Position += right * 0.1;

		if (Keyboard['a'])
			cam.transform.Position += -right * 0.1;

		if (Keyboard[' '])
			cam.transform.Position += up * 0.1;

		if (Keyboard[KeyCode::LEFT_CONTROL])
			cam.transform.Position += -up * 0.1;

		//cam.transform.Position.Print();

		//Mouse.GetDelta().Print();

		//t.Position.x = sin(f) * 2.0f;

		//t.Rotation = Vector3(0.f, f, 0.f);

		Matrix4 ModelTransform = t.updateTransform();

		Matrix4 pv_M = cam.getPVMatrix();
		Matrix4 mvp = (pv_M * ModelTransform);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glUniformMatrix4fv(MVPMatrixUniform, 1, false, mvp);
		//glUniformMatrix4fv(ModelMatrixUniform, 1, false, t.localTransform);

		glPolygonMode(GL_FRONT, GL_FILL);

		glDrawElements(GL_TRIANGLES, indxCount, GL_UNSIGNED_INT, 0);
		
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glDrawElements(GL_TRIANGLES, indxCount, GL_UNSIGNED_INT, 0);


		glBindVertexArray(0);

		glfwSwapBuffers(window);
		
		MouseCallbackHelper::Callback(window);
	}

	delete[] verts;
	delete[] indxs;

	Keyboard.DestroyKeyManager();
	Mouse.DestroyMouseManager();

	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &IBO);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}