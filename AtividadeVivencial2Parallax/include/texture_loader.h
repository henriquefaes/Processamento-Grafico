#pragma once
#include <glad/glad.h>

struct Texture {
    GLuint id;
    int width;
    int height;
};

Texture loadTexture(const char* path);
