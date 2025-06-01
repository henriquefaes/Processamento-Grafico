#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "texture_loader.h"
#include "quad_renderer.h"
#include "parallax.h"

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "Parallax Scene", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1600, 0, 1000, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::vector<Layer> layers = {
        Layer(loadTexture("assets/background/layer06_sky.png").id,     0.0f, 1920.0f),
        Layer(loadTexture("assets/background/layer05_rocks.png").id,   0.1f, 1920.0f),
        Layer(loadTexture("assets/background/layer04_clouds.png").id,  0.3f, 1920.0f),
        Layer(loadTexture("assets/background/layer03_trees.png").id,   0.5f, 1920.0f),
        Layer(loadTexture("assets/background/layer02_cake.png").id,    0.8f, 1920.0f),
        Layer(loadTexture("assets/background/layer01_ground.png").id,  1.0f, 1920.0f)
    };

    Texture playerTexture = loadTexture("assets/characters/player.png");
    float playerX = 400.0f;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) playerX -= 2.0f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) playerX += 2.0f;

        renderParallax(layers.data(), layers.size(), playerX);

        renderPlayer(playerTexture.id, 500);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
