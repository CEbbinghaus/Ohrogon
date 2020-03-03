#include <crtdbg.h>
#include <gl_core_4_5.h>
#include <glfw3.h>
#include <chrono>
#include <iostream>
//#include <atyp_All.h>
#include <atyp_Array.h>
#include <atyp_Math.h>
#include <atyp_Threading.h>
#include <atyp_Transform.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Camera.h"
#include "KeyManager.h"
#include "Material.h"
#include "Mesh.h"
#include "ModelLoader.h"
#include "MouseManager.h"
#include "Primitive.h"
#include "Shader.h"
#include "Time.h"
#include "Window.h"
#include "test.h"
#include "Color.h"

using uint = unsigned int;
using Clock = std::chrono::steady_clock;

// bool IsActive = true;
//
// void window_focus_callback(GLFWwindow* window, int focused){
//	if(focused){
//		IsActive = true;// The window gained input focus
//	} else{
//		IsActive = false;
//		// The window lost input focus
//	}
//}

int main() {
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

  if (glfwInit() == false) return -1;

  GLFWwindow *window =
      glfwCreateWindow(1920, 1080, "Ohrogon Engine", nullptr, nullptr);

  if (window == nullptr) {
    glfwTerminate();
    return -2;
  }

  glfwMakeContextCurrent(window);

  if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
    glfwDestroyWindow(window);
    glfwTerminate();
    return -3;
  }

  // glfwSetWindowFocusCallback(window, window_focus_callback);

  auto major = ogl_GetMajorVersion();
  auto minor = ogl_GetMinorVersion();
  printf("Running OpenGL Version %i.%i\n", major, minor);

  stbi_set_flip_vertically_on_load(true);

  // turn VSync off
  // glfwSwapInterval(0);

  // Key Manager Initialization
  KeyManager &Keyboard = KeyManager::CreateKeyManager(window);

  // Mouse Configuration Options
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  if (glfwRawMouseMotionSupported())
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

  // Mouse Initialization
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

  uint VertShader =
      shader.LoadShader("./Shaders/VertShader.vert", Shader::Type::Vertex);
  uint FragShader =
      shader.LoadShader("./Shaders/AltFragShader.frag", Shader::Type::Frag);

  shader.CompileShader();

  // Shader shader2 = Shader();

  // shader2.CompileShader({ VertShader, FragShader })

  Mesh prim = ModelLoader::LoadObj("./Meshes/Orb.obj");
  //prim.RecalculateNormals();

  prim.CalculateTangents();

  prim.transform.Scale = Vector3(5.0f);
  // prim.RecalculateNormals();

  // prim.RecalculateNormals();

  //prim.transform.Position = Vector3::forward() * -10.0f;
  // prim.FlatShade();
  // prim.SetIndices(prim.Indices);

  // Mesh plane = Primitive::Plane(1);
  // Mesh cylinder = Primitive::Cylinder(20);
  // Mesh prim = Primitive::Cube();
  int width, height, nrChannels;
  unsigned char *imageData =
      stbi_load("./Textures/Texture.png", &width, &height, &nrChannels, 0);

  if (!imageData) throw "Couldnt Find Image";

  uint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glActiveTexture(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int Channels = nrChannels == 3 ? GL_RGB : GL_RGBA;
  glTexImage2D(GL_TEXTURE_2D, 0, Channels, width, height, 0, Channels,
               GL_UNSIGNED_BYTE, imageData);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(imageData);


  Material m = Material(shader.ProgrammID);

  bool Wireframe = false;

  bool FlatShaded = false;

  glUseProgram(shader.ProgrammID);

  glClearColor(0.10f, 0.10f, 0.12f, 0);

  auto MVPMatrixUniform = glGetUniformLocation(shader.ProgrammID, "MVPMatrix");
  auto ModelMatrixUniform =
      glGetUniformLocation(shader.ProgrammID, "ModelMatrix");
  auto CameraPosition =
      glGetUniformLocation(shader.ProgrammID, "cameraPosition");
  // auto TextureUniform = glGetUniformLocation(shader.ProgrammID, "Texture");

  Camera cam;

  cam.fov = 1.207f;
  cam.aspectRatio = 16 / 9.0f;
  cam.NearPlane = 0.01f;
  cam.FarPlane = 100000.0f;

  // plane.transform.Scale = Vector3::one() * 10.0f;
  // plane.transform.Position = Vector3(-.5f, 0, -.5f);

  float f = 0.0f;
  Vector2 camRotation(0.0f, 0.0f);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);
  glPolygonMode(GL_FRONT, GL_FILL);

  while (glfwWindowShouldClose(window) == false &&
         !(Keyboard.altKey && Keyboard.shiftKey &&
           Keyboard[KeyCode::DELETE_Key])) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // if(IsActive){
    glfwPollEvents();
    Keyboard.Update();
    //}

    Time::Update();

    if (Keyboard[KeyCode::ENTER] && Keyboard.altKey && Keyboard.shiftKey) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
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

    f += Time::DeltaTime;

    // prim.transform.Rotation = Vector3::up() * f;

    if (f > 0.5f) camRotation += (Mouse.GetDelta() * 0.005f) * Time::OneTime;

    camRotation.y = Math::clamp(camRotation.y, -1.2f, 1.2f);

    cam.transform.Rotation =
        Quaternion::euler(camRotation.y, camRotation.x, 0.0f);

    Vector3 forward = cam.transform.forward();
    Vector3 right = cam.transform.right();
    Vector3 up = Vector3::up();

    Vector3 movement;

    if (Keyboard['w']) movement += forward * 0.1;

    if (Keyboard['s']) movement += -forward * 0.1;

    if (Keyboard['d']) movement += -right * 0.1;

    if (Keyboard['a']) movement += right * 0.1;

    if (Keyboard[' ']) movement += up * 0.1;

    if (Keyboard[KeyCode::LEFT_CONTROL]) movement += -up * 0.1;

    if (Keyboard.pressed['m']) {
      if (Wireframe) {
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT, GL_FILL);
        Wireframe = false;
      } else {
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        Wireframe = true;
      }
    }

    if (Keyboard.pressed['l']) {
      if (FlatShaded) {
        prim.FlatShade();
        FlatShaded = false;
      } else {
        prim.SmoothShade();
        FlatShaded = true;
      }
    }

    if (Keyboard[KeyCode::ESCAPE]) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    cam.transform.Position += movement * Time::OneTime;

    Matrix4 ModelMatrix = prim.transform.updateTransform();
    Matrix4 pv_M = cam.getPVMatrix();
    glUniformMatrix4fv(MVPMatrixUniform, 1, false, pv_M * ModelMatrix);
    glUniformMatrix4fv(ModelMatrixUniform, 1, false, ModelMatrix);
    glUniform3fv(CameraPosition, 1, (GLfloat*)&cam.transform.Position);
    // glUniform1i(TextureUniform, )

    // plane.draw(MVPMatrixUniform, pv_M);

    // cylinder.draw(MVPMatrixUniform, pv_M);

    // m.Bind();
    // float Time = fmodf(Time::TotalTime * 10.0, 360.0f) / 360.0f;
    // m.Ka = HSLtoRGB(Time, .2f, .2f);

    m.Bind();
    glBindTexture(GL_TEXTURE_2D, texture);
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