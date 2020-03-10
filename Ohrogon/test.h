#include <atyp_Vectors.h>
#include "ArrayStructBuffer.h"

// struct BaseLight{
//   BaseLight base;
//};

//uniform int LightCount;
struct PointLight{
  Vector3 color = Vector3(0.0f, .5f, 1.0f);
  float intensity = 1.0f;
  Vector3 position = Vector3();
  float radius = 5.0f;
};

// struct BufferData {
//   Vector3 position = Vector3();
//   Vector3 color = Vector3(0.0f, .5f, 1.0f);
//   float intensity = 1.0f;
// };

class DataBuffer: public ArrayStructBuffer<PointLight>{
    public:
    DataBuffer(uint ProgramID): ArrayStructBuffer(ProgramID, "PointLight", "PointLightCount", 4){

  //       add({ Vector3(.0f, 2.0f, .0f), Vector3(.0f, .5f, 1.0f), 1.0f });
  //  /*     (*this)[0].position = Vector3(.0f, 2.0f, .0f);
  //       (*this)[0].color = Vector3(.0f, .5f, 1.0f);
  //       (*this)[0].intensity = 1.0f;*/
  //       //(*this)[0].color;

        Specify<Vector3>("color");
        Specify<float>("intensity");
        Specify<Vector3>("position");
        Specify<float>("radius");

        (*this)[1].color = Vector3(0.0f, 0.5f, 1.0f);

        length = 2;
  //       Bind();
    }
};