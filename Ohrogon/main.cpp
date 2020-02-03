#include <gl_core_4_5.h>
#include <glfw3.h>
#include <iostream>
#include <atyp_All.h>
#include "Transform.h"

using uint = unsigned int;

int main() {
	if (glfwInit() == false)
		return -1;

	GLFWwindow* window = glfwCreateWindow(1280, 1280, "Ohrogon Engine", nullptr, nullptr);

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

	Vector3 points[] = {
		Vector3(0.0, 0.0, 0.0),
		Vector3(1.0, 0.0, 0.0),
		Vector3(1.0, 1.0, 0.0),
		Vector3(0.0, 1.0, 0.0)
	};

	Vector3 verticies[] = {
		points[0] - 0.5 * Vector3::one(),
		points[1] - 0.5 * Vector3::one(),
		points[2] - 0.5 * Vector3::one(),
		points[0] - 0.5 * Vector3::one(),
		points[2] - 0.5 * Vector3::one(),
		points[3] - 0.5 * Vector3::one(),
	};

	Vector3 movedVerticies[] = {
		points[0] - 0.5 * Vector3::one(),
		points[1] - 0.5 * Vector3::one(),
		points[2] - 0.5 * Vector3::one(),
		points[0] - 0.5 * Vector3::one(),
		points[2] - 0.5 * Vector3::one(),
		points[3] - 0.5 * Vector3::one(),
	};

	uint VAO;
	uint VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vector3), &verticies, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Transform t;

	t.Rotation = Vector3(0.f, 0.f, 0.f);


	float f = 0.0f;

	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		f += 0.01;

		t.Rotation = Vector3(0.f, 0.f, f);

		t.updateLocalTransform();

		for (int i = 0; i < 6; ++i) {
			movedVerticies[i] = t.localTransform * Vector4(verticies[i]);
		}



		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vector3), &movedVerticies, GL_STATIC_DRAW);


		glClearColor(0.30f, 0.30f, 0.40f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}