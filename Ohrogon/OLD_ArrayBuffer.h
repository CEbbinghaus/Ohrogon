#pragma once;

#include <gl_core_4_5.h>
#include <atyp_Array.h>

using uint = unsigned int;

struct test{
    int index;
    float f;
};

template <
    typename T,
    class = typename std::enable_if_t<std::is_class_v<T>, T>>
class ArrayStructBuffer
{
private:
    class SubBuffer;
    Array<SubBuffer> BufferArray;

    struct Variable{
        uint size;
        const char *name;
        const void *location;
        uint index;
        uint offset;
    };
    Array<Variable> variables;
    
    uint memoryCounter = 0;

    static uint HighestBindIndex;

    const char *name;
    uint ProgramID;
    int dataSize;

    class SubBuffer : public T{
        const ArrayStructBuffer<T>* parent;
    public:
        uint index;
        uint blockIndex;
        uint buffer;
        void* data;

        SubBuffer(const ArrayStructBuffer<T>* p, const T& object): parent(p){
            memcpy(this, &object, sizeof(T));
            index = parent->BufferArray.length;
            std::string location = std::string(parent->name) + "[" + std::to_string(index) + "]";
            blockIndex = glGetUniformBlockIndex(parent->ProgramID, location.c_str());

            Initialize();
        }

        ~SubBuffer(){
            if(data != nullptr)
                free(data);
        }

        //  SubBuffer(const SubBuffer& other){
        //      memcpy(this, &other, sizeof(SubBuffer));
        //      data = malloc(dataSize);
        //      memcpy(data, other.data, dataSize);
        //  }

        //  SubBuffer(const SubBuffer&& other){
        //      memcpy(this, &other, sizeof(SubBuffer));
        //      other->data = nullptr;
        //  }

        //  void operator =(const SubBuffer& other){
        //      memcpy(this, &other, sizeof(SubBuffer));
        //      data = malloc(dataSize);
        //      memcpy(data, other.data, dataSize);
        //  }

        //  void operator =(const SubBuffer&& other){
        //      memcpy(this, &other, sizeof(SubBuffer));
        //      other->data = nullptr;
        //  }

        void Initialize(){
            glUniformBlockBinding(parent->ProgramID, blockIndex, index);

            glGenBuffers(1, &buffer);

            data = malloc(parent->dataSize);
        }

        void Bind(){
            glUniformBlockBinding(parent->ProgramID, blockIndex, index);

            for(int i = 0; i < parent->variables.length; ++i){
                ArrayStructBuffer::Variable ref = parent->variables.Get(i);
                //memcpy((char*)data + v.offset, (char*)this + v.location, v.size);
            }


            glBindBuffer(GL_UNIFORM_BUFFER, buffer);
            glBufferData(GL_UNIFORM_BUFFER, parent->dataSize, data, GL_STATIC_DRAW);
            glBindBufferRange(GL_UNIFORM_BUFFER, index, buffer, 0, parent->dataSize);
        }
    };

    void AllocateData()
    {
        glGetActiveUniformBlockiv(ProgramID, glGetUniformBlockIndex(ProgramID, name), GL_UNIFORM_BLOCK_DATA_SIZE, &dataSize);
    }

    void GetDataLocation(const char* name, uint& index, uint& offset){
        glGetUniformIndices(ProgramID, 1, (const GLchar* const*)&name, (GLuint*)&index);

        glGetActiveUniformsiv(ProgramID, 1, (GLuint*)&index,
                                  GL_UNIFORM_OFFSET, (GLint*)&offset);

        if(index == -1 || offset == -1)
            throw "Could Not Run Stuff";

        return;
    }

protected:
    ArrayStructBuffer(uint ShaderProgram, const char *name) : name(name)
    {
        glGetError();

        ProgramID = ShaderProgram;

        AllocateData();
    }

    /* Specify<T>(const char* name)
     *  
     * template:
     *      T = The Type of the Variable 
     * Args:
     *      name - Name of Variable in Shader
     */
    template <class ValueType>
    void Specify(const char *name)
    {

        std::string finalName = (std::string(this->name) + ".") + name;
        

        uint index, offset;
        GetDataLocation(finalName.c_str(), index, offset);

        uint byteCount = (uint)sizeof(ValueType);
        variables.push({byteCount, finalName.c_str(), (void *)((char *)this + memoryCounter), index, offset});
        memoryCounter += byteCount;
    }

public:

    void Bind(){
        for(int i = 0; i < BufferArray.length; ++i){
            BufferArray[i].Bind();
          // b.Bind();
        }
    }

    void add(const T& data){
        SubBuffer tmp(this, data);
        BufferArray.push(tmp);
    }

    T& operator[](int index){
        if(index >= BufferArray.size())throw "Out of Bounds";
        Array<SubBuffer>& arr = (*((Array<SubBuffer>*)this));
        return *((T*)&arr[index]);
    }
};

template <typename T, class V>
uint ArrayStructBuffer<T, V>::HighestBindIndex = 1;

/*
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

    std::vector<Variable> variables;

    void AllocateData()
    {
        std::string location = std::string(name) + "[1]";
        blockIndex = glGetUniformBlockIndex(ProgramID, location.c_str());
        bindIndex = HighestBindIndex++;

        glGetActiveUniformBlockiv(ProgramID, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &dataSize);

        glUniformBlockBinding(ProgramID, blockIndex, bindIndex);
        
        // glGetActiveUniformBlockiv(ProgramID,
        //     blockIndex,
        //     GL_UNIFORM_BLOCK_BINDING,
        //     &bindIndex);

        glGenBuffers(1, &buffer);

        data = malloc(dataSize * 2);
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
    ArrayStructBuffer(uint ShaderProgram, const char *name, bool instanced = false) : name(name)
    {
        glGetError();

        ProgramID = ShaderProgram;
        isInstanced  = instanced;

        AllocateData();
    }

    ArrayStructBuffer(const ArrayStructBuffer &original)
    {
        memcpy((T *)this, (T *)&original, sizeof(T));
        name = original.name;
        ProgramID = original.ProgramID;
        isInstanced = original.isInstanced;
        AllocateData();
    }

    ~ArrayStructBuffer(){
        free(data);
    }

    /* Specify<T>(const char* name)
     *  
     * template:
     *      T = The Type of the Variable 
     * Args:
     *      name - Name of Variable in Shader
     
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

        glUniformBlockBinding(ProgramID, blockIndex, bindIndex);

        for (int i = 0; i < variables.length; ++i)
        {
            auto& v = variables[i];
            memcpy((char*)data + v.offset, v.location, v.size);
            memcpy(((char*)data + dataSize) + v.offset, v.location, v.size);

            //printf("Index: %i Offset: %i\n", v.index, v.offset);
        }


        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
        glBufferData(GL_UNIFORM_BUFFER, dataSize * 2, data, GL_STATIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, bindIndex, buffer, 0, dataSize * 2);
    }
*/