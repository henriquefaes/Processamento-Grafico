#pragma once
#include "Sprite.h"
#include <GLFW/glfw3.h>

class CharacterController : public Sprite {
public:
    CharacterController(const char* imagePath);
    void processInput(GLFWwindow* window);
    void update(float deltaTime);

private:
    int tileX, tileY;
    float tileSize = 0.12f;
    float frameTime = 0.15f;
    float timeAccumulator = 0.0f;
    bool moving = false;
    bool keyPressedLastFrame = false;
};
