#pragma once;

#include <gl_core_4_5.h>

#include <atyp_Vector4.h>
#include <atyp_Array.h>

using uint = unsigned int;

enum class Type{
    vec1 = 1,
    vec2 = 2,
    vec3 = 3,
    vec4 = 4,
    mat2 = 4,
    mat3 = 9,
    mat4 = 16
};

template <
    typename T,
    class = typename std::enable_if_t<std::is_class_v<T>, T>
>
class StructBuffer: public T{
    struct variable{
        Type type;
        const char* name;
        uint offset;
    };

    static uint HighestBindIndex;
    uint memoryCounter = 0;


    const char* name;
    uint ProgramID;

    uint bindIndex;
    uint blockIndex;
    uint bufferIndex;

    Array<variable> variables;

    void AllocateData(){
        variables = Array<variables>();
        blockIndex = glGetUniformBlockIndex(ProgramID, name);

        uint Error = glGetError();
        if(Error == GL_INVALID_OPERATION)throw "Programm id is Invalid or Has not yet been Linked";
        if(Error == GL_INVALID_INDEX)throw "Name Does not exist in Shader Programm";

        bindIndex = HighestBindIndex++;
    }
public:

    StructBuffer(uint ShaderProgramm, const char* name): name(name){
        glGetError();
        
        ProgramID = ShaderProgramm;

        AllocateData();
    }

    StructBuffer(const StructBuffer& original){
        memcpy((T*)this,  (T*)&original, sizeof(T));
        name = original.name;
        ProgramID = original.ProgramID;

        AllocateData();
    }   

    /* Specify(Type t, const char* name)
     *  
     * Args:
     *      t - Type of the Vaiable
     *      name - Name of Variable in Shader
     */
    void Specify(Type t, const char* name){
        variables.push(StructBuffer(t, name, memoryCounter));
        memoryCounter += (uint)t;
    }

    /* Specify(Type t, uint offset, const char* name)
     *  
     * Args:
     *      t - Type of the Vaiable
     *      offset - Location of Variable relative from the beginning of the struct
     *      name - Name of Variable in Shader
     */
    void Specify(Type t, uint offset, const char* name){
        variables.push(StructBuffer(t, name, offset));
        memoryCounter = offset;
    }

};

template<typename T, class V>
uint StructBuffer<T, V>::HighestBindIndex = 0;