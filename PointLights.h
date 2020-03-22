#pragma once

#include <atyp_Vectors.h>
#include "StructBuffer.h"

struct PointLight{
  Vector3 color = Vector3(0.0f, .5f, 1.0f);
  float intensity = 1.0f;
  Vector3 position = Vector3();
  float radius = 5.0f;
};

class PointLights: public ArrayStructBuffer<PointLight>{
    public:
    PointLights(uint ProgramID, uint amount): ArrayStructBuffer(ProgramID, "PointLight", "PointLightCount", amount){
        Specify<Vector3>("color");
        Specify<float>("intensity");
        Specify<Vector3>("position");
        Specify<float>("radius");
    }
};