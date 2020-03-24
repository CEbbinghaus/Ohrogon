#pragma once;

#include <atyp_Vectors.h>
#include "StructBuffer.h"

struct DirectionLight {
  Vector3 color = Vector3::one();
  float intensity = 1.0f;
  Vector3 direction = -Vector3::up();
};

class DirectionLights : public  ArrayStructBuffer<DirectionLight> {\
public:
    DirectionLights(uint ProgramID, uint amount): ArrayStructBuffer<DirectionLight>(ProgramID, "DirectionLight", "DirectionLightCount", amount){
        Specify<Vector3>("color");
        Specify<float>("intensity");
        Specify<Vector3>("direction");
        length = 0;
    }
};