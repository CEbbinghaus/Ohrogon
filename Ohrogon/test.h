#include <atyp_Vectors.h>
#include "ArrayStructBuffer.h"

struct BufferData {
  Vector3 position;
  Vector3 color;
  float intensity;
};

class DataBuffer: public ArrayStructBuffer<BufferData>{
    public:
    DataBuffer(uint ProgramID): ArrayStructBuffer(ProgramID, "LightBlock"){

        add({ Vector3(.0f, 2.0f, .0f), Vector3(.0f, .5f, 1.0f), 1.0f });
   /*     (*this)[0].position = Vector3(.0f, 2.0f, .0f);
        (*this)[0].color = Vector3(.0f, .5f, 1.0f);
        (*this)[0].intensity = 1.0f;*/
        //(*this)[0].color;

        Specify<Vector3>("position");
        Specify<Vector3>("color");
        Specify<float>("intensity");
        Bind();
    }
};