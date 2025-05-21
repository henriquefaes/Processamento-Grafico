#include "parte1.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

static const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
)";

static const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(0.2, 0.8, 0.3, 1.0);
    }
)";

GLuint createTriangle(float x0, float y0, float x1, float y1, float x2, float y2) {
    float vertices[] = {
        x0, y0,
        x1, y1,
        x2, y2
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    return VAO;
}

GLuint compilarShaderProgram() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void runParte1() {
    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar GLFW.\n";
        return;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Parte 1 - Triangulos Fixos", nullptr, nullptr);
    if (!window) {
        std::cerr << "Erro ao criar janela GLFW.\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Erro ao inicializar GLAD.\n";
        return;
    }

    GLuint shaderProgram = compilarShaderProgram();

    std::vector<GLuint> triVAOs = {
        createTriangle(-0.9f, -0.5f, -0.7f, -0.5f, -0.8f, -0.3f),
        createTriangle(-0.5f, -0.5f, -0.3f, -0.5f, -0.4f, -0.3f),
        createTriangle(-0.1f, -0.5f, 0.1f, -0.5f, 0.0f, -0.3f),
        createTriangle(0.3f, -0.5f, 0.5f, -0.5f, 0.4f, -0.3f),
        createTriangle(0.7f, -0.5f, 0.9f, -0.5f, 0.8f, -0.3f)
    };

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        for (GLuint vao : triVAOs) {
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
