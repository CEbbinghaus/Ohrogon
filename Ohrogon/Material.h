#pragma once
#include <atyp_Vector3.h>
#include <gl_core_4_5.h>
#include "StructBuffer.h"

using uint = unsigned int;

struct MaterialProperties{
    Vector3 Ka; // ambient material colour
    Vector3 Kd; // diffuse material colour
    Vector3 Ks; // specular material colour
    Vector3 Ia; // ambient light colour
    Vector3 Id; // diffuse light colour
    Vector3 Is; // specular light colour
    Vector3 LightDirection;
    float specularPower; // material specular power
};

class Material : public StructBuffer<MaterialProperties>{
public:
    Material(uint programm): StructBuffer(programm, "Material", true){
        Ka = Vector3::one() / 2;
        Kd = Vector3::one();
        Ks = Vector3::one();
        Ia = Vector3::one() * 0.1;
        Id = Vector3::one();
        Is = Vector3::one();

        LightDirection = -Vector3::right();

        specularPower = 32.0f;

        Specify<Vector3>("Ka");
        Specify<Vector3>("Kd");
        Specify<Vector3>("Ks");
        Specify<Vector3>("Ia");
        Specify<Vector3>("Id");
        Specify<Vector3>("Is");
        Specify<Vector3>("LightDirection");
        Specify<float>("specularPower");
    }
    
};
