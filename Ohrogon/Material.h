#pragma once
#include <atyp_Vector3.h>
#include "gl_core_4_5.h"

using uint = unsigned int;

class Material{
public:
    Vector3 Ka; // ambient material colour
    Vector3 Kd; // diffuse material colour
    Vector3 Ks; // specular material colour
    float specularPower; // material specular power
    Vector3 Ia; // ambient light colour
    Vector3 Id; // diffuse light colour
    Vector3 Is; // specular light colour
    Vector3 LightDirection;

    uint programmid;
    Material(uint programm){
        programmid = programm;

        Ka = Vector3::one() * 0.1;
        Kd = Vector3::one() * 0.2;
        Ks = Vector3::one() * 0.1;
        Ia = Vector3::one() * 0.25f;
        Id = Vector3::one() - Vector3::forward();
        Is = Vector3::one() - Vector3::forward();

        LightDirection = Vector3::up();
        specularPower = .001f;
    }
    
    void Bind(){
        uint bindingPoint = 1, buffer, blockIndex;
        blockIndex = glGetUniformBlockIndex(programmid, "Material");
        glUniformBlockBinding(programmid, blockIndex, bindingPoint);

        if((int)blockIndex == -1)
            #ifdef _DEBUG
                throw "Could not find Block Index";
            #else
                return;
            #endif

        glGenBuffers(1, &buffer);
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);

        glBufferData(GL_UNIFORM_BUFFER, sizeof(Material), this, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, buffer);
    }
};