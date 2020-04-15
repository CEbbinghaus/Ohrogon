#pragma once;

#include <gl_core_4_5.h>
#include <atyp_Array.h>

#include "String.h"

using uint = unsigned int;
uint HighestBindIndex = 0;


template <
    typename T,
    class = typename std::enable_if_t<std::is_class_v<T>, T>>
class StructBuffer : public T{
  private:
    struct Variable
    {
        uint size;
        const char *name;
        const void *location;
        uint index;
        uint offset;
    };
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

    void AllocateData(){
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
    StructBuffer(uint ShaderProgram, const char *name, bool instanced = false) : name(name){
        glGetError();

        ProgramID = ShaderProgram;
        isInstanced  = instanced;

        AllocateData();
    }

    StructBuffer(const StructBuffer &original){
        memcpy((T *)this, (T *)&original, sizeof(T));
        name = original.name;
        ProgramID = original.ProgramID;
        isInstanced = original.isInstanced;
        AllocateData();
    }

    ~StructBuffer(){
        glDeleteBuffers(1, &buffer);

        if(data != nullptr)
            free(data);
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

        std::string finalName = isInstanced ? std::string(this->name) + std::string(".") + std::string(name) : std::string(name);
        

        uint index, offset;
        GetDataLocation(finalName.c_str(), index, offset);

        uint byteCount = (uint)sizeof(ValueType);
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
    template <class ValueType>
    void Specify(const char *name, const void *location)
    {

        std::string finalName = isInstanced ? std::string(this->name) + std::string(".") + std::string(name) : std::string(name);

        uint index, offset;
        GetDataLocation(finalName.c_str(), index, offset);

        uint byteCount = (uint)sizeof(ValueType);
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


//TODO: Fix up Memory Leaks


template <typename T>
class ArrayStructBuffer {
   private:
    struct Variable {
        uint size;
        uint index;
        uint offset;
        const char* name;
        uint location;
    };
    Array<Variable> variables;
    uint memoryOffset;

    uint ProgramID;
    const char* name;
    const char* lengthName;
    uint lengthLocation;
    int dataSize;
    uint maxLength;

    class StructContainer : public T {
        ArrayStructBuffer* parent;

       public:
        uint index;
        uint blockIndex;
        uint bindIndex;
        uint buffer = -1;
        void* data = nullptr;

        StructContainer(ArrayStructBuffer* p) : parent(p) {}

        StructContainer(const StructContainer& v) : parent(v.parent) {
            parent = v.parent;
            if (data != nullptr)
                free(data);
            data = malloc(parent->dataSize);
            memcpy(data, v.data, parent->dataSize);
            //Initialize(v.index);
        }

        StructContainer(StructContainer&& v) noexcept {
            parent = v.parent;
            data = v.data;
            v.data = nullptr;
            //Initialize(v.index);
        }

        void operator=(const StructContainer& v) {
            parent = v.parent;
            if (data != nullptr) free(data);
            data = malloc(parent->dataSize);
            if (v.data != nullptr)
                memcpy(data, v.data, parent->dataSize);
            //Initialize(v.index);
        }

        void operator=(StructContainer&& v) noexcept {
            parent = v.parent;
            data = v.data;
            v.data = nullptr;
            Initialize(v.index);
        }

        void Initialize(uint index) {
            //glDeleteBuffers(1, &buffer);
            
            this->bindIndex = HighestBindIndex++;

            String location = String::format("%s[%i]", parent->name, index);

            blockIndex = glGetUniformBlockIndex(parent->ProgramID, location);

            glUniformBlockBinding(parent->ProgramID, blockIndex, bindIndex);

            glGenBuffers(1, &buffer);

            if(data != nullptr)
                free(data);

            data = malloc(parent->dataSize);
        }

        ~StructContainer() {
            glDeleteBuffers(1, &buffer);
            if (data != nullptr)
                free(data);
            data = nullptr;
        }

        void Bind() {
            glUniformBlockBinding(parent->ProgramID, blockIndex, bindIndex);

            for (uint i = 0; i < parent->variables.length; ++i) {
                auto& v = parent->variables[i];
                memcpy((char*)data + (int)v.offset, (char*)this + v.location, v.size);
            }

            glBindBuffer(GL_UNIFORM_BUFFER, buffer);
            glBufferData(GL_UNIFORM_BUFFER, parent->dataSize, data, GL_STATIC_DRAW);
            glBindBufferRange(GL_UNIFORM_BUFFER, bindIndex, buffer, 0, parent->dataSize);
        }
    };
    Array<StructContainer> structs;

    void InitializeSize() {
        uint index = glGetUniformBlockIndex(ProgramID, name);
        lengthLocation = glGetUniformLocation(ProgramID, lengthName);

        glGetActiveUniformBlockiv(ProgramID, index, GL_UNIFORM_BLOCK_DATA_SIZE, &dataSize);
    }

    void GetDataLocation(const char* name, uint& index, uint& offset) {
        glGetUniformIndices(ProgramID, 1, (const GLchar* const*)&name, (GLuint*)&index);

        glGetActiveUniformsiv(ProgramID, 1, (GLuint*)&index, GL_UNIFORM_OFFSET, (GLint*)&offset);

        if (index == -1 || offset == -1)
            throw "Could Not Run Stuff";

        return;
    }

   protected:

    ArrayStructBuffer(uint Programid, const char* name, const char* lengthName, uint maxLength) : ProgramID(Programid), name(name), lengthName(lengthName) {
        InitializeSize();
        this->maxLength = maxLength;
        length = 0;
        structs = Array<StructContainer>(maxLength).fill(StructContainer(this));
        for (int i = 0; i < maxLength; ++i) {
            structs[i].Initialize(i);
        }
        //structs.push(StructContainer(this));
    }

    template <typename ValueType>
    void Specify(const char* varName) {
        String finalName = String::format("%s.%s", name, varName);

        uint index, offset;
        GetDataLocation(finalName, index, offset);

        uint size = sizeof(ValueType);

        variables.push({size, index, offset, finalName, memoryOffset});

        memoryOffset += size;
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
    void Specify(const char* variableName, const void* location) {
        String finalName = String::format("%s.%s", name, variableName);

        uint index, offset;
        GetDataLocation(finalName, index, offset);

        memoryOffset = (uint)(this - location);

        uint size = (uint)sizeof(ShaderType);
        variables.push({size, index, offset, finalName, memoryOffset});

        //Side Effect. Will Cause Issues if not Properly Used
    }

   public:
    uint length;
    void Bind() {
        glUniform1i(lengthLocation, length);
        for(int i = 0; i < length; ++i){
            structs[i].Bind();
        }
        // for (StructContainer& s : structs) {
        //     s.Bind();
        // }
    }

    T& operator[](unsigned int index) {
        if (index >= maxLength) throw "Out of Bounds Index";
        return (T&)structs[index];
    }
};