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
#include "Keyboard.h"
#include "Material.h"
#include "Mesh.h"
#include "ModelLoader.h"
#include "Mouse.h"
#include "Primitive.h"
#include "Shader.h"
#include "Time.h"
#include "Window.h"
#include "PointLights.h"
#include "Color.h"
#include "String.h"
#include "DirectionLights.h"
#include "Console.h"
#include "Object.h"
#include "Game.h"

using uint = unsigned int;
using Clock = std::chrono::steady_clock;

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

int main() {
  #ifdef _DEBUG
    #define _CRTDBG_MAP_ALLOC
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  #endif


  if (glfwInit() == false) return -1;

  GLFWwindow *window =
      glfwCreateWindow(1920, 1080, "Ohrogon Engine", nullptr, nullptr);

    Game::CreateInstance(window);

  if (window == nullptr) {
    glfwTerminate();
    return -2;
  }

  if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
    glfwDestroyWindow(window);
    glfwTerminate();
    return -3;
  }


  auto major = ogl_GetMajorVersion();
  auto minor = ogl_GetMinorVersion();
  printf("Running OpenGL Version %i.%i\n", major, minor);

  stbi_set_flip_vertically_on_load(true);
  // turn VSync off
  // glfwSwapInterval(0);

  Shader shader = Shader();

  uint VertShader =
      shader.LoadShader("./data/Shaders/VertShader.vert", ShaderType::Vertex);
  uint FragShader =
      shader.LoadShader("./data/Shaders/FragShader.frag", ShaderType::Frag);

  shader.CompileShader();

  Object* test1 = new Object();


  Mesh prim = Primitive::Sphere(20, 20);
  prim.RecalculateNormals();

  prim.transform.Scale = Vector3(.1f);
  prim.transform.Position = Vector3(.0f, .0f, -.2f);

  Camera cam;

  cam.fov = 1.207f;
  cam.aspectRatio = 16 / 9.0f;
  cam.NearPlane = 0.01f;
  cam.FarPlane = 100000.0f;
  cam.type = CameraType::Orthographic;


  glUseProgram(shader.ProgrammID);

  glClearColor(0.10f, 0.10f, 0.12f, 0);

  auto MVPMatrixUniform = glGetUniformLocation(shader.ProgrammID, "MVPMatrix");
  auto ModelMatrixUniform =
      glGetUniformLocation(shader.ProgrammID, "ModelMatrix");
  auto CameraPosition =
      glGetUniformLocation(shader.ProgrammID, "cameraPosition");

  uint diffuseUniform = glGetUniformLocation(shader.ProgrammID, "DiffuseTexture");
  uint normalUniform  = glGetUniformLocation(shader.ProgrammID, "NormalTexture");

  // Then bind the uniform samplers to texture units:
  glUniform1i(diffuseUniform, 0);
  glUniform1i(normalUniform,  1);
  float f = 0.0f;
  Vector2 camRotation(0.0f, 0.0f);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);
  glPolygonMode(GL_FRONT, GL_FILL);

  while (glfwWindowShouldClose(window) == false) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();
    Keyboard::Update();

    Time::Update();

    Vector3 right =  Vector3::right();
    Vector3 up = Vector3::up();

    Vector3 movement;

    if (Keyboard::GetKey(KeyCode::W)) movement += up;

    if (Keyboard::GetKey(KeyCode::S)) movement += -up;

    if (Keyboard::GetKey(KeyCode::D)) movement += right;

    if (Keyboard::GetKey(KeyCode::A)) movement += -right;

    cam.transform.Position += movement * (float)Time::DeltaTime * 10.0f;

    if(Keyboard::GetKeyDown(KeyCode::F5))
      shader.Reload();

    if (Keyboard::GetKeyDown(KeyCode::ESCAPE)) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (Keyboard::GetKeyDown(KeyCode::C)) {
      if(cam.type == CameraType::Perspective)
        cam.type = CameraType::Orthographic;
      else
        cam.type = CameraType::Perspective;
    }

    Matrix4 pv_M = cam.getPVMatrix();
    glUniform3fv(CameraPosition, 1, (GLfloat*)&cam.transform.Position);

    prim.transform.Rotation = Quaternion::euler(0, Time::TotalTime /  Math::PI(), 0);

    // {
    //   diffuseUniform = glGetUniformLocation(shader.ProgrammID, "DiffuseTexture");
    //   // Then bind the uniform samplers to texture units:
    //   glUniform1i(diffuseUniform, 0);
    //   glActiveTexture(GL_TEXTURE0);
    //   glBindTexture(GL_TEXTURE_2D, texture);
    // }

    //sphere.draw(MVPMatrixUniform, ModelMatrixUniform, pv_M);
    prim.draw(MVPMatrixUniform, ModelMatrixUniform, pv_M);
    
    glfwSwapBuffers(window);

    Mouse::Update();
  }

  Game::DestroyInstance();

  _CrtDumpMemoryLeaks();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}