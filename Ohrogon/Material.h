#pragma once
#include <atyp_Vector3.h>
#include <atyp_Vector4.h>
#include "gl_core_4_5.h"

using uint = unsigned int;

struct MaterialProperties{
    Vector4 Ka; // ambient material colour
    Vector4 Kd; // diffuse material colour
    Vector4 Ks; // specular material colour
    Vector4 Ia; // ambient light colour
    Vector4 Id; // diffuse light colour
    Vector4 Is; // specular light colour
    Vector4 LightDirection;
    float specularPower; // material specular power
};

class Material : MaterialProperties{
public:
    uint programmid;
    uint bindingPoint = 1, buffer, blockIndex;
    Material(uint programm){
        programmid = programm;

        Ka = Vector4(0.5f, .0f, 0.5f);
        Kd = Vector4::one();
        Ks = Vector4::one();
        Ia = Vector4::one();
        Id = Vector4::one() - Vector4::forward();
        Is = Vector4::one() - Vector4::forward();

        LightDirection = Vector4::one();

        specularPower = 50.0f;

        LightDirection.Print();

        blockIndex = glGetUniformBlockIndex(programmid, "Material");
        glUniformBlockBinding(programmid, blockIndex, bindingPoint);

        if((int)blockIndex == -1)
            #ifdef _DEBUG
                throw "Could not find Block Index";
            #else
                return;
            #endif

        glGenBuffers(1, &buffer);
    }
    
    void Bind(){
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialProperties), &Ka, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, buffer);
    }
};