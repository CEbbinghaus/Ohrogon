#include <gl_core_4_5.h>
#include <glfw3.h>
#include <iostream>
//#include <atyp_All.h>
#include "Shader.h"
#include "atyp_Transform.h"
#include "Camera.h"
#include "Window.h"

using uint = unsigned int;

int main() {

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



	Vector3 points[] = {
		Vector3(-.5f, -.5f, .0f),
		Vector3(.5f, -.5f,  .0f),
		Vector3(.5f, .5f,   .0f),
		Vector3(-.5f, .5f,  .0f)
	};

	Vector3 vertices[] = {
		points[0],
		points[2],
		points[1],

		points[0],
		points[2],
		points[3],
	};

	//int indexs[6] = { 0, 1, 2, 0, 2, 3 };

	uint VAO;
	uint VBO;
	//uint IBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &IBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vector3), &vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, IBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), &indexs, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(shader.ProgrammID);

	glClearColor(0.30f, 0.30f, 0.40f, 1);
	//glClearColor(0.0f, 0.0f, 0.0f, 0);


	auto MVPMatrixUniform = glGetUniformLocation(shader.ProgrammID, "MVPMatrix");
	//auto ModelMatrixUniform = glGetUniformLocation(shader.ProgrammID, "ModelMatrix");

	Camera cam;

	cam.fov = 1.507f;
	cam.aspectRatio = 16 / 9.0f;
	cam.NearPlane = 0.1f;
	cam.FarPlane = 50.0f;

	cam.position.z = -1.0f;

	Transform t;

	//t.Scale = Vector3::one() * 0.1;

	t.updateLocalTransform();

	t.Position.z = -1.0f;

	////t.Rotation = Vector3(0.f, 0.f, 0.f);
	//cam.getProjectionMatrix().Print();

	//cam.getViewMatrix().Print();

	//t.localTransform.Print();

	/*(cam.getPVMatrix() * t.localTransform).Print();*/

	t.localTransform.Print();

	float f = 0.0f;
	//for (int i = 0; i < 6; ++i) {
	//	Vector4 scaledVert = mvp * Vector4(vertices[i], 1.0f);
	//	std::cout << scaledVert.x << " " << scaledVert.y << " " << scaledVert.z << std::endl;
	//}

	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
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

		t.Rotation = Vector3(0.f, f, 0.f);

		t.updateLocalTransform();

		Matrix4 pv_M = cam.getPVMatrix();
		Matrix4 mvp = (pv_M * t.localTransform);

		glUniformMatrix4fv(MVPMatrixUniform, 1, false, mvp);
		//glUniformMatrix4fv(ModelMatrixUniform, 1, false, t.localTransform);



		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vector3), &vertices, GL_STATIC_DRAW);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indexs);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &IBO);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}