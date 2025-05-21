#include "parte2.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

struct Triangle {
    glm::vec2 position;
    glm::vec3 color;
};

GLuint VAO, VBO;
std::vector<Triangle> triangles;

static const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    uniform vec2 offset;
    void main() {
        gl_Position = vec4(aPos + offset, 0.0, 1.0);
    }
)";

static const char* fragmentShaderSource = R"(
    #version 330 core
    uniform vec3 inputColor;
    out vec4 FragColor;
    void main() {
        FragColor = vec4(inputColor, 1.0);
    }
)";

GLuint compilarShaderProgramParte2() {
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

void setupBaseTriangle() {
    float vertices[] = {
        -0.1f, -0.1f,
         0.1f, -0.1f,
         0.0f,  0.1f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        int width, height;
        glfwGetCursorPos(window, &xpos, &ypos);
        glfwGetWindowSize(window, &width, &height);

        float ndc_x = (xpos / width) * 2.0f - 1.0f;
        float ndc_y = 1.0f - (ypos / height) * 2.0f;

        glm::vec2 pos(ndc_x, ndc_y);
        glm::vec3 color(static_cast<float>(rand()) / RAND_MAX,
            static_cast<float>(rand()) / RAND_MAX,
            static_cast<float>(rand()) / RAND_MAX);
        triangles.push_back({ pos, color });
    }
}

void runParte2() {
    srand(static_cast<unsigned int>(time(nullptr)));

    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar GLFW.\n";
        return;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Parte 2 - Clique + Cor Aleatoria", nullptr, nullptr);
    if (!window) {
        std::cerr << "Erro ao criar janela GLFW.\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Erro ao carregar GLAD.\n";
        return;
    }

    GLuint shaderProgram = compilarShaderProgramParte2();
    setupBaseTriangle();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        for (const auto& tri : triangles) {
            GLint offsetLoc = glGetUniformLocation(shaderProgram, "offset");
            GLint colorLoc = glGetUniformLocation(shaderProgram, "inputColor");
            glUniform2f(offsetLoc, tri.position.x, tri.position.y);
            glUniform3f(colorLoc, tri.color.r, tri.color.g, tri.color.b);

            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
