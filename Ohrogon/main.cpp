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

	{
		int x, y;
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glfwGetWindowPos(window, &x, &y);
		Window::Position.x = x;
		Window::Position.y = y;
		Window::Width = width;
		Window::Height = height;
		Window::aspectRatio = ((float)width / height);
	}

	Shader shader("./Shaders/VertShader.glsl", "./Shaders/FragShader.glsl");



	Vector3 points[] = {
		Vector3(-.5f, -.5f, .0f),
		Vector3(.5f, -.5f,  .0f),
		Vector3(.5f, .5f,   .0f),
		Vector3(-.5f, .5f,  .0f)
	};

	Vector3 vertices[] = {
		points[0],
		points[1],
		points[2],

		points[0],
		points[2],
		points[3],
	};

	int indexs[6] = { 0, 1, 2, 0, 2, 3 };

	uint VAO;
	uint VBO;
	uint IBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vector3), &vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, IBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(int), &indexs, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(shader.ProgrammID);
	glClearColor(0.30f, 0.30f, 0.40f, 1);


	auto PVMatrixUniform = glGetUniformLocation(shader.ProgrammID, "MVPMatrix");
	auto ModelMatrixUniform = glGetUniformLocation(shader.ProgrammID, "ModelMatrix");

	Camera cam;

	cam.fov = 1.507f;
	cam.aspectRatio = 16 / 9.0f;
	cam.NearPlane = 0.1f;
	cam.FarPlane = 50.0f;

	cam.position.z = -1.0f;

	Transform t;

	t.updateLocalTransform();

	//t.Rotation = Vector3(0.f, 0.f, 0.f);
	cam.getProjectionMatrix().Print();

	cam.getViewMatrix().Print();

	t.localTransform.Print();

	(cam.getVPMatrix() * t.localTransform).Print();

	float f = 0.0f;

	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		{
			int x, y;
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			glfwGetWindowPos(window, &x, &y);
			Window::Position.x = x;
			Window::Position.y = y;
			Window::Width = width;
			Window::Height = height;
			Window::aspectRatio = ((float)width / height);
		}

		f += 0.01f;

		t.Rotation = Vector3(0.f, 0.f, 0.f);

		t.updateLocalTransform();

		//for (int i = 0; i < 6; ++i) {
		//	movedVerticies[i] = t.localTransform * Vector4(verticies[i]);
		//}

		Matrix4 pv_M = cam.getVPMatrix();
		Matrix4 mvp = Matrix4();//(t.localTransform * pv_M);
		glUniformMatrix4fv(PVMatrixUniform, 1, false, pv_M);
		glUniformMatrix4fv(ModelMatrixUniform, 1, false, mvp);



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

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}