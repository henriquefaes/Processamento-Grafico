#pragma once
#include <glad/glad.h>
#include <cmath>

struct Layer {
    GLuint texture;
    float speed;
    float width;

    Layer(GLuint tex, float spd, float w) {
        texture = tex;
        speed = spd;
        width = w;
    }
};

void renderParallax(const Layer* layers, int layerCount, float offsetX);
void renderPlayer(GLuint texture, float x);
