#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include <string>

class Sprite {
public:
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;

    Sprite(const std::string& texturePath, glm::vec2 position, glm::vec2 scale, float rotation, Shader* shader);
    Sprite(GLuint texture, glm::vec2 position, glm::vec2 scale, float rotation, Shader* shader,
        glm::vec2 subOffset, glm::vec2 subSize, glm::vec2 sheetSize);

    void draw();

    static GLuint loadTexture(const std::string& path);

private:
    GLuint VAO, VBO;
    GLuint textureID;
    Shader* shader;
    glm::vec2 texOffset;
    glm::vec2 texSize;
    glm::vec2 texSheetSize;
    bool useSubTexture;

    void initRenderData();
};
