#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

const int WIDTH = 800, HEIGHT = 600;

struct Vertex {
    float x, y;
    float r, g, b;
};

std::vector<Vertex> vertices;
GLuint VBO, VAO;
GLuint shaderProgram;

float randomColor() {
    return static_cast<float>(rand()) / RAND_MAX;
}

const char* vertexShaderSrc = R"glsl(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec3 aColor;
    out vec3 ourColor;
    void main() {
        gl_Position = vec4(aPos.x / 400.0 - 1.0, -(aPos.y / 300.0 - 1.0), 0.0, 1.0);
        ourColor = aColor;
    }
)glsl";

const char* fragmentShaderSrc = R"glsl(
    #version 330 core
    in vec3 ourColor;
    out vec4 FragColor;
    void main() {
        FragColor = vec4(ourColor, 1.0);
    }
)glsl";

GLuint createShaderProgram() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    return program;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        Vertex v;
        v.x = static_cast<float>(xpos);
        v.y = static_cast<float>(ypos);

        if (vertices.size() % 3 == 0) {
            v.r = randomColor();
            v.g = randomColor();
            v.b = randomColor();
        }
        else {
            Vertex last = vertices.back();
            v.r = last.r;
            v.g = last.g;
            v.b = last.b;
        }

        vertices.push_back(v);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    }
}

int main() {
    srand((unsigned int)time(nullptr));
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Triângulos Interativos", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();

    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    shaderProgram = createShaderProgram();

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);       // posição
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float))); // cor
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.9f, 0.9f, 0.95f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}
