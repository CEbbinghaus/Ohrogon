#include <atyp_Vectors.h>
#include "StructBuffer.h"

struct data {
  Vector3 a;
  Vector3 b;
};

class DataBuffer: public StructBuffer<data>{
    public:
    DataBuffer(uint ProgramID): StructBuffer(ProgramID, "ColorBlock"){
        a = Vector3(.0f, .5f, 1.0f);
        b = Vector3(1.0f, 1.0f, 1.0f);

        Specify<Vector3>("diffuse");
        Specify<Vector3>("ambient");
        Bind();
    }
};