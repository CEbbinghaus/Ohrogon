#include <gl_core_4_5.h>
#include <glfw3.h>
#include <iostream>
#include <chrono>
//#include <atyp_All.h>
#include <atyp_Transform.h>
#include <atyp_Math.h>
#include <atyp_Array.h>
#include <atyp_Threading.h>

#include "Shader.h"
#include "Camera.h"
#include "Window.h"
#include "KeyManager.h"
#include "MouseManager.h"
#include "Mesh.h"
#include "Primitive.h"
#include "Time.h"
#include "ModelLoader.h"

using uint = unsigned int;
using Clock = std::chrono::steady_clock;

int main(){
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if(glfwInit() == false)
		return -1;

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Ohrogon Engine", nullptr, nullptr);

	if(window == nullptr){
		glfwTerminate(); return -2;
	}

	glfwMakeContextCurrent(window);

	if(ogl_LoadFunctions() == ogl_LOAD_FAILED){
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
	KeyManager& Keyboard = KeyManager::CreateKeyManager(window);

	//Mouse Configuration Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if(glfwRawMouseMotionSupported())
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

	Shader shader = Shader();

	uint VertShader = shader.LoadShader("./Shaders/VertShader.shader", Shader::Type::Vertex);
	uint FragShader = shader.LoadShader("./Shaders/FragShader.shader", Shader::Type::Frag);

	shader.CompileShader();

	//Shader shader2 = Shader();

	//shader2.CompileShader({ VertShader, FragShader })


	//Mesh plane = Primitive::Plane(1);
	//Mesh cylinder = Primitive::Cylinder(20);
	//Mesh prim = Primitive::Cube();

	Mesh prim = Primitive::Cube();
	//prim = ModelLoader::LoadObj("./Meshes/bear.obj");
	
	//Mesh* m{};
	//bool done = false;

	//void (*func)(Mesh*){
	//	[](Mesh* m){
	//		Mesh mesh = ModelLoader::LoadObj("./Meshes/teapot.obj");
	//		m = new Mesh(mesh);
	//		printf("Mesh Successfully loaded\n");
	//	}
	//};

	//Thread<Mesh> t = Thread(func, done, m);
	


	//prim.FlatShade();
	//prim.SmoothShade();

	//prim.transform.Scale = Vector3(10.0f);


	bool Wireframe = false;
	
	bool FlatShaded = false;

	glUseProgram(shader.ProgrammID);

	glClearColor(0.30f, 0.30f, 0.40f, 1);

	auto MVPMatrixUniform = glGetUniformLocation(shader.ProgrammID, "MVPMatrix"); 
	//auto ModelMatrixUniform = glGetUniformLocation(shader.ProgrammID, "ModelMatrix");

	Camera cam;

	cam.fov = 1.207f;
	cam.aspectRatio = 16 / 9.0f;
	cam.NearPlane = 0.01f;
	cam.FarPlane = 100000.0f;

	//plane.transform.Scale = Vector3::one() * 10;
	//plane.transform.Position = Vector3(-.5f, 0, -.5f);

	float f = 0.0f;
	Vector2 camRotation(0.0f, 0.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT, GL_FILL);

	while (glfwWindowShouldClose(window) == false && !(Keyboard.altKey && Keyboard.shiftKey && Keyboard[KeyCode::DELETE_Key])) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		Keyboard.Update();

		Time::Update();


		/*if(done){
			printf("Done Flag set\n");
			done = false;
			prim = *m;
		}*/

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

		if(Keyboard.pressed['m']){
			if (Wireframe) {
				glEnable(GL_CULL_FACE);
				glPolygonMode(GL_FRONT, GL_FILL);
				Wireframe = false;
			}else {
				glDisable(GL_CULL_FACE);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				Wireframe = true;
			}
		}


		if (Keyboard.pressed['l']) {
			if (FlatShaded) {
				prim.FlatShade();
				FlatShaded = false;
			}
			else {
				prim.SmoothShade();
				FlatShaded = true;
			}
		}

		if(Keyboard[KeyCode::ESCAPE]){
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		cam.transform.Position += movement * Time::OneTime;

		Matrix4 pv_M = cam.getPVMatrix();

		//plane.draw(MVPMatrixUniform, pv_M);

		//cylinder.draw(MVPMatrixUniform, pv_M);

		prim.draw(MVPMatrixUniform, pv_M);

		glfwSwapBuffers(window);
		
		MouseCallbackHelper::Callback(window);
	}

	Keyboard.DestroyKeyManager();
	Mouse.DestroyMouseManager();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}