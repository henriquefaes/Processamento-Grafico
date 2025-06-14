#include "map_data.h"
#include "tilemap.h"
#include "utils.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "player.h"

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

Player player;

int map[MAP_ROWS][MAP_COLS] = {
    {4, 4, 4, 1, 1, 0, 0, 0},
    {4, 4, 1, 1, 1, 1, 0, 0},
    {4, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {5, 1, 1, 1, 1, 1, 1, 3},
    {5, 5, 1, 1, 1, 1, 3, 3},
    {5, 5, 5, 1, 1, 3, 3, 3}
};
int mapOriginal[MAP_ROWS][MAP_COLS] = {
    {4, 4, 4, 1, 1, 0, 0, 0},
    {4, 4, 1, 1, 1, 1, 0, 0},
    {4, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {5, 1, 1, 1, 1, 1, 1, 3},
    {5, 5, 1, 1, 1, 1, 3, 3},
    {5, 5, 5, 1, 1, 3, 3, 3}
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_W: player.move(-1, 0); break;
        case GLFW_KEY_S: player.move(1, 0); break;
        case GLFW_KEY_A: player.move(0, -1); break;
        case GLFW_KEY_D: player.move(0, 1); break;
        case GLFW_KEY_Q: player.move(-1, -1); break;
        case GLFW_KEY_E: player.move(-1, 1); break;
        case GLFW_KEY_Z: player.move(1, -1); break;
        case GLFW_KEY_C: player.move(1, 1); break;
        }
    }
}


int main() {
    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tilemap Isometrico", nullptr, nullptr);
    if (!window) {
        std::cerr << "Erro ao criar janela GLFW\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Erro ao inicializar GLEW\n";
        return -1;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    GLuint shaderProgram = loadShader("shaders\\shader.vert", "shaders\\shader.frag");
    GLuint texture = loadTexture("assets\\tilesetIso.png");

    map[player.row][player.col] = 6;

    while (!glfwWindowShouldClose(window)) {
        glfwSetKeyCallback(window, key_callback);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawTilemap(shaderProgram, texture, player.row, player.col);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}
