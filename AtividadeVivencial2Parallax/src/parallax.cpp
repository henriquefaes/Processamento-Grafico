#include "parallax.h"
#include "quad_renderer.h"
#include <cmath>

void renderParallax(const Layer* layers, int layerCount, float offsetX) {
    for (int i = 0; i < layerCount; ++i) {
        float layerSpeed = layers[i].speed;
        float scrollX = -offsetX * layerSpeed;

        float texWidth = layers[i].width;
        float startX = fmod(scrollX, texWidth);
        if (startX > 0) startX -= texWidth;


        for (float x = startX; x < 1600; x += texWidth) {
            drawTexturedQuad(layers[i].texture, x, 0, texWidth, 1080);
        }
    }
}

void renderPlayer(GLuint texture, float x) {
    drawTexturedQuad(texture, x, 100, 400, 400);
}
