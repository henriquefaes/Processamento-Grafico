#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "sprite.h"
#include "shader.h"
#include <vector>

const int WIDTH = 800;
const int HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Sprites", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);


    shader.use();
    shader.setMat4("projection", projection);

    Sprite background("assets/background/orig.png", glm::vec2(WIDTH / 2, HEIGHT / 2), glm::vec2(WIDTH, HEIGHT), 0.0f, &shader);

    std::vector<Sprite> sprites;
    GLuint sheetTexture = Sprite::loadTexture("assets/sprites/peixes.png");
    for (int i = 0; i < 8; ++i) {
        int row = i / 4;
        int col = i % 4;
        float texX = col * 32.0f;
        float texY = row * 32.0f;
        glm::vec2 pos(rand() % WIDTH, rand() % HEIGHT);
        sprites.emplace_back(sheetTexture, pos, glm::vec2(64, 64), 0.0f, &shader,
            glm::vec2(texX, texY), glm::vec2(32, 32), glm::vec2(128, 64));
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        background.draw();

        for (auto& sprite : sprites) {
            sprite.position.x += 0.5f;
            if (sprite.position.x > WIDTH + sprite.scale.x / 2) {
                sprite.position.x = -sprite.scale.x / 2;
            }
            sprite.draw();
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}