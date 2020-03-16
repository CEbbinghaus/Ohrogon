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
#include "PointLights.h"
#include "Color.h"
#include "String.h"
#include "DirectionLights.h"

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
      shader.LoadShader("./data/Shaders/VertShader.vert", ShaderType::Vertex);
  uint FragShader =
      shader.LoadShader("./data/Shaders/FragShader.frag", ShaderType::Frag);

  shader.CompileShader();

  // Shader shader2 = Shader();

  // shader2.CompileShader({ VertShader, FragShader })
  Mesh sphere = Primitive::Sphere(20, 20);
  sphere.RecalculateNormals();
  Mesh prim = ModelLoader::LoadObj("./data/Objects/Orb/Orb.obj");
  prim.RecalculateNormals();

  prim.transform.Scale = Vector3(5.0f);
  sphere.transform.Scale = Vector3(5.0f);
  sphere.transform.Position = Vector3(0.0f, 0.0f, 10.0f);
  // prim.RecalculateNormals();

  // prim.RecalculateNormals();

  //prim.transform.Position = Vector3::forward() * -10.0f;
  // prim.FlatShade();
  // prim.SetIndices(prim.Indices);

  // Mesh plane = Primitive::Plane(1);
  // Mesh cylinder = Primitive::Cylinder(20);
  // Mesh prim = Primitive::Cube();
  uint texture, texture1;
  {
    int width, height, nrChannels;
    unsigned char *imageData =
        stbi_load("./data/Objects/Orb/Diffuse.png", &width, &height, &nrChannels, 0);

    if (!imageData) throw "Couldnt Find Image";

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    int Channels = nrChannels == 3 ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, Channels, width, height, 0, Channels,
                GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imageData);
  }
  {
    int width, height, nrChannels;
    unsigned char *imageData =
        stbi_load("./data/Objects/Orb/Normal.png", &width, &height, &nrChannels, 0);

    if (!imageData) throw "Couldnt Find Image";

    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    int Channels = nrChannels == 3 ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, Channels, width, height, 0, Channels, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imageData);
  }

  Material m = Material(shader.ProgrammID);

  DirectionLights dirLights = DirectionLights(shader, 4);
  dirLights.length = 2.0f;
  dirLights[0].direction = Vector3::right();
  dirLights[0].intensity = 1.0f;
  dirLights[1].direction = -Vector3::up();
  dirLights[1].color = Vector3(1.0f, 1.0f, 1.0f);
  dirLights[1].intensity = .7f;

  PointLights ptLights = PointLights(shader, 8);
  ptLights.length = 1;
  ptLights[0].color = Vector3::one();
  ptLights[0].intensity = 1.0f;
  ptLights[0].position = Vector3(5.0f, 0.0f, 1.0f);


  //DataBuffer LightArray = DataBuffer(shader.ProgrammID);


  bool Wireframe = false;

  bool FlatShaded = false;

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

  Camera cam;

  cam.fov = 1.207f;
  cam.aspectRatio = 16 / 9.0f;
  cam.NearPlane = 0.01f;
  cam.FarPlane = 100000.0f;

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

    if(Keyboard.pressed[KeyCode::F5])
      shader.Reload();

    if (Keyboard[KeyCode::ESCAPE]) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    cam.transform.Position += movement * Time::OneTime;

    Matrix4 pv_M = cam.getPVMatrix();
    glUniform3fv(CameraPosition, 1, (GLfloat*)&cam.transform.Position);

    prim.transform.Rotation = Quaternion::euler(0, Time::TotalTime /  Math::PI(), 0);

    float Time = fmodf(Time::TotalTime * 100.0, 360.0f) / 360.0f;
    dirLights[0].color = HSLtoRGB(Time, 1.0f, .5f);
    ptLights[0].position = Vector3(sinf(Time::TotalTime * 2) * 5.0f, 0.0f, cosf(Time::TotalTime * 2) * 5.0f);

    ptLights.Bind();
    dirLights.Bind();

    m.Bind();

    {

      diffuseUniform = glGetUniformLocation(shader.ProgrammID, "DiffuseTexture");
      normalUniform  = glGetUniformLocation(shader.ProgrammID, "NormalTexture");

      // Then bind the uniform samplers to texture units:
      glUniform1i(diffuseUniform, 0);
      glUniform1i(normalUniform,  1);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texture1);
    }

    sphere.draw(MVPMatrixUniform, ModelMatrixUniform, pv_M);
    prim.draw(MVPMatrixUniform, ModelMatrixUniform, pv_M);
    
    glfwSwapBuffers(window);

    MouseCallbackHelper::Callback(window);
  }

  Keyboard.DestroyKeyManager();
  Mouse.DestroyMouseManager();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}