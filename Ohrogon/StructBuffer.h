#pragma once;

#include <gl_core_4_5.h>
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
    bool isInstanced;
    uint ProgramID;

    int bindIndex = 1;
    uint blockIndex;
    int dataSize;
    uint buffer;

    void* data;

    Array<Variable> variables;

    void AllocateData()
    {
        blockIndex = glGetUniformBlockIndex(ProgramID, name);
        bindIndex = HighestBindIndex++;

        glGetActiveUniformBlockiv(ProgramID, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &dataSize);

        glUniformBlockBinding(ProgramID, blockIndex, bindIndex);
        
        // glGetActiveUniformBlockiv(ProgramID,
        //     blockIndex,
        //     GL_UNIFORM_BLOCK_BINDING,
        //     &bindIndex);

        glGenBuffers(1, &buffer);

        data = malloc(dataSize);
    }

    void GetDataLocation(const char* name, uint& index, uint& offset){
        glGetUniformIndices(ProgramID, 1, (const GLchar* const *)&name, (GLuint*)&index);

        glGetActiveUniformsiv(ProgramID, 1, (GLuint*)&index,
                                  GL_UNIFORM_OFFSET, (GLint*)&offset);

        if(index == -1 || offset == -1)
            throw "Could Not Run Stuff";

        return;
     }

protected:
    StructBuffer(uint ShaderProgram, const char *name, bool instanced = false) : name(name)
    {
        glGetError();

        ProgramID = ShaderProgram;
        isInstanced  = instanced;

        AllocateData();
    }

    StructBuffer(const StructBuffer &original)
    {
        memcpy((T *)this, (T *)&original, sizeof(T));
        name = original.name;
        ProgramID = original.ProgramID;
        isInstanced = original.isInstanced;
        AllocateData();
    }

    ~StructBuffer(){
        free(data);
    }

    /* Specify<T>(const char* name)
     *  
     * template:
     *      T = The Type of the Variable 
     * Args:
     *      name - Name of Variable in Shader
     */
    template <class ShaderType>
    void Specify(const char *name)
    {

        std::string finalName = isInstanced ? std::string(this->name) + std::string(".") + std::string(name) : std::string(name);
        

        uint index, offset;
        GetDataLocation(finalName.c_str(), index, offset);

        uint byteCount = (uint)sizeof(ShaderType);
        variables.push({byteCount, finalName.c_str(), (void *)((char *)this + memoryCounter), index, offset});
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
    template <class ShaderType>
    void Specify(const char *name, const void *location)
    {

        std::string finalName = isInstanced ? std::string(this->name) + std::string(".") + std::string(name) : std::string(name);

        uint index, offset;
        GetDataLocation(finalName.c_str(), index, offset);

        uint byteCount = (uint)sizeof(ShaderType);
        variables.push({ byteCount, finalName.c_str(), location, index, offset });

        //Side Effect. Will Cause Issues if not Properly Used
        memoryCounter = offset;
    }

public:
    void Bind()
    {
        for (int i = 0; i < variables.length; ++i)
        {
            auto& v = variables[i];
            memcpy((char*)data + v.offset, v.location, v.size);

            //printf("Index: %i Offset: %i\n", v.index, v.offset);
        }


        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
        glBufferData(GL_UNIFORM_BUFFER, dataSize, data, GL_STATIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, bindIndex, buffer, 0, dataSize);
    }
};

template <typename T, class V>
uint StructBuffer<T, V>::HighestBindIndex = 1;