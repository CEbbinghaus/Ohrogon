#pragma once

#include <stb_image.h>
#include <gl_core_4_5.h>

using uint = unsigned int;

enum class ChannelCount : int{
    RGB = 3,
    RGBA = 4
};

class Texture{
    unsigned char* data;
    int width, height;
    ChannelCount channels;
    const char* file;
    uint texture;


    Texture(const char* path){
        file = path;
        data = stbi_load(file, &width, &height, (int*)&channels, 0);
        if (!data) throw "Couldnt Find Image";

        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB + ((int)channels - 3), width, height, 0, GL_RGB + ((int)channels - 3),
                    GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    operator const unsigned char*(){
        return data;
    }
};