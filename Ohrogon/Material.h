#pragma once
#include <atyp_Vector3.h>
#include "gl_core_4_5.h"
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
    uint bindingPoint = 1, buffer, blockIndex;
    Material(uint programm): StructBuffer(programm, "Material"){

        Ka = Vector3(0.5f, .0f, 0.5f);
        Kd = Vector3::one();
        Ks = Vector3::one();
        Ia = Vector3::one();
        Id = Vector3::one() - Vector3::forward();
        Is = Vector3::one() - Vector3::forward();

        LightDirection = Vector3::up();

        specularPower = 50.0f;

        // LightDirection.Print();

        // blockIndex = glGetUniformBlockIndex(programmid, "Material");
        // glUniformBlockBinding(programmid, blockIndex, bindingPoint);

        // if((int)blockIndex == -1)
        //     #ifdef _DEBUG
        //         throw "Could not find Block Index";
        //     #else
        //         return;
        //     #endif

        // glGenBuffers(1, &buffer);


        // GLint blockSize;
        // glGetActiveUniformBlockiv(programmid, blockIndex,
        // GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

        // // Query for the offsets of each block variable
        // const GLchar *names[] = { "Ka", "Kd",
        // "Ks", "Ia", "Id", "Is", "LightDirection", "specularPower" };

        // GLuint indices[8];
        // glGetUniformIndices(programmid, 8, names, indices);

        // GLint offsets[8];
        // glGetActiveUniformsiv(programmid, 8, indices,
        // GL_UNIFORM_OFFSET, offsets);
        
        // for(int i = 0; i < 8; ++i){
        //     int index = indices[i];
        //     int offset = offsets[i];
        //     printf("Index: %i Offset: %i", index, offset);
        // }

        //TODO: Properly Bind Uniform Block Data
        //glGetActiveUniformBlockiv(programmid, blockIndex, )
    }
    
    // void Bind(){
    //     glBindBuffer(GL_UNIFORM_BUFFER, buffer);
    //     glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialProperties), &Ka, GL_STATIC_DRAW);
    //     glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, buffer);
    // }
};