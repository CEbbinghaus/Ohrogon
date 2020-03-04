#include <stb_image.h>

using uint = unsigned int;

enum ChannelCount : int{
    RGB = 3,
    RGBA = 4
};

class Texture{
    unsigned char* data;
    int width, height;
    ChannelCount channels;
    const char* file;


    Texture(const char* path){
        file = path;
        data = stbi_load(file, &width, &height, (int*)&channels, 0);
    }
};