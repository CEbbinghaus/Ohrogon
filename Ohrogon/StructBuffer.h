#pragma once;

#include <gl_core_4_5.h>

#include <atyp_Vector4.h>
#include <atyp_Array.h>

using uint = unsigned int;

template <
    typename T,
    class = typename std::enable_if_t<std::is_class_v<T>, T>>
class StructBuffer : public T
{
private:
    struct Variable
    {
        uint size;
        const char *name;
        const void *location;
        uint index;
        uint offset;
    };

    static uint HighestBindIndex;
    uint memoryCounter = 0;

    const char *name;
    uint ProgramID;

    int bindIndex;
    uint blockIndex;
    uint bufferSize;
    uint bufferIndex;

    void* buffer;

    Array<Variable> variables;

    void AllocateData()
    {
        blockIndex = glGetUniformBlockIndex(ProgramID, name);

        printf("Block Index is: %i\n", blockIndex);

        uint Error = glGetError();

        if (Error == GL_INVALID_OPERATION)
            throw "Programm id is Invalid or Has not yet been Linked";
        if (Error == GL_INVALID_INDEX)
            throw "Name Does not exist in Shader Programm";

        glGetActiveUniformBlockiv(ProgramID, blockIndex,
        GL_UNIFORM_BLOCK_DATA_SIZE, (GLint*)&bufferSize);

        Error = glGetError();
        
        glGetActiveUniformBlockiv(ProgramID,
            blockIndex,
            GL_UNIFORM_BLOCK_BINDING,
            &bindIndex);

        printf("Binding Point is: %i\n", bindIndex);

        Error = glGetError();

        buffer = malloc(bufferSize);
        glGenBuffers(1, &bufferIndex);

        glUniformBlockBinding(ProgramID, blockIndex, bindIndex);

        Error = glGetError();

        if(Error != GL_NO_ERROR)throw "SOMETHING";
    }

    void GetDataLocation(const char* name, uint& index, uint& offset){
        glGetUniformIndices(ProgramID, 1, (const GLchar* const *)&name, (GLuint*)&index);

        glGetActiveUniformsiv(ProgramID, 1, (GLuint*)&index,
                                  GL_UNIFORM_OFFSET, (GLint*)&offset);
                            
    }

protected:
    StructBuffer(uint ShaderProgramm, const char *name) : name(name)
    {
        glGetError();

        ProgramID = ShaderProgramm;

        AllocateData();
    }

    StructBuffer(const StructBuffer &original)
    {
        memcpy((T *)this, (T *)&original, sizeof(T));
        name = original.name;
        ProgramID = original.ProgramID;

        AllocateData();
    }

    ~StructBuffer(){
        free(buffer);
    }

    /* Specify<T>(const char* name)
     *  
     * template:
     *      T = The Type of the Variable 
     * Args:
     *      name - Name of Variable in Shader
     */
    template <class Type>
    void Specify(const char *name)
    {
        uint index, offset;
        GetDataLocation(name, index, offset);

        uint byteCount = (uint)sizeof(Type);
        variables.push({byteCount, name, (void *)((char *)this + memoryCounter), index, offset});
        memoryCounter += byteCount;
    }

    /* Specify<T>(const char* name, void* location)
     *  
     * template:
     *      T = The Type of the Variable 
     * 
     * Args:
     *      name - Name of Variable in Shader
     *      location - location of the Variable
     */
    template <class Type>
    void Specify(const char *name, const void *location)
    {
        uint index, offset;
        GetDataLocation(name, index, offset);

        uint byteCount = (uint)sizeof(Type);
        variables.push({ byteCount, name, location, index, offset });

        //Side Effect. Will Cause Issues if not Properly Used
        memoryCounter = offset;
    }

public:
    void Bind()
    {
        // GLuint bindingPoint = 1, buffer, blockIndex;
        // float myFloats[8] = {1.0, 0.0, 0.0, 1.0,   0.4, 0.0, 0.0, 1.0};
        
        // blockIndex = glGetUniformBlockIndex(ProgramID, "ColorBlock");
        // glUniformBlockBinding(ProgramID, blockIndex, bindingPoint);
        
        // glGenBuffers(1, &buffer);
        // glBindBuffer(GL_UNIFORM_BUFFER, buffer);
        
        // glBufferData(GL_UNIFORM_BUFFER, sizeof(myFloats), myFloats, GL_DYNAMIC_DRAW);
        // glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, buffer);
        // for(int i = 0; i < bufferSize / 4; ++i){
        //     ((float*)buffer)[i] = 1.0f;
        // }
        // //TODO: Push the Data to the Graphics Card
        // // for (int i = 0; i < variables.length; ++i)
        // // {
        // //     auto& v = variables[i];
        // //     memcpy((char*)buffer + v.offset, v.location, v.size);

        // //     printf("Index: %i Offset: %i\n", v.index, v.offset);
        // // }

        // glBindBuffer(GL_UNIFORM_BUFFER, bufferIndex);
        // glBufferData(GL_UNIFORM_BUFFER, bufferSize, &buffer, GL_DYNAMIC_DRAW);
        // glBindBufferBase(GL_UNIFORM_BUFFER, bindIndex, bufferIndex);
    }
};

template <typename T, class V>
uint StructBuffer<T, V>::HighestBindIndex = 1;