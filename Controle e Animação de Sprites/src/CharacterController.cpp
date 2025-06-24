#include "CharacterController.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

CharacterController::CharacterController(const char* imagePath)
    : Sprite(imagePath, 4, 3) {
    tileX = 5;
    tileY = 5;
    x = (tileX - tileY) * tileSize * 0.5f;
    y = (tileX + tileY) * tileSize * 0.25f;

    currentFrame = 0;
    currentAnimation = 2;
    updateTexCoords();
}


void CharacterController::processInput(GLFWwindow* window) {
    if (moving) return;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        tileX += 2;
        currentAnimation = 0;
        moving = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        tileY -= 2;
        currentAnimation = 1;
        moving = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        tileX -= 2;
        currentAnimation = 2;
        moving = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        tileY += 2;
        currentAnimation = 3;
        moving = true;
    }

    if (moving) {
        currentFrame = 0;
        timeAccumulator = 0.0f;

        x = (tileX - tileY) * tileSize * 0.5f;
        y = (tileX + tileY) * tileSize * 0.25f;

        updateTexCoords();
    }
}

void CharacterController::update(float deltaTime) {
    if (!moving) return;

    timeAccumulator += deltaTime;

    if (timeAccumulator >= frameTime) {
        timeAccumulator = 0.0f;

        currentFrame = (currentFrame + 1) % nFrames;
        updateTexCoords();

        if (currentFrame == 0) {
            moving = false;
        }
    }
}

