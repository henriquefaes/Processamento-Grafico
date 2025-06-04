#define STB_IMAGE_IMPLEMENTATION
#include "sprite.h"
#include <stb_image.h>
#include <iostream>

Sprite::Sprite(const std::string& texturePath, glm::vec2 pos, glm::vec2 scl, float rot, Shader* shd)
    : position(pos), scale(scl), rotation(rot), shader(shd), useSubTexture(false) {
    textureID = loadTexture(texturePath);
    initRenderData();
}

Sprite::Sprite(GLuint tex, glm::vec2 pos, glm::vec2 scl, float rot, Shader* shd,
    glm::vec2 subOffset, glm::vec2 subSize, glm::vec2 sheetSize)
    : position(pos), scale(scl), rotation(rot), shader(shd),
    textureID(tex), texOffset(subOffset), texSize(subSize), texSheetSize(sheetSize), useSubTexture(true) {
    initRenderData();
}

void Sprite::initRenderData() {
    float vertices[] = {
        -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  1.0f, 1.0f,

         0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Sprite::draw() {
    shader->use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scale, 1.0f));
    shader->setMat4("model", model);

    if (useSubTexture) {
        glm::vec2 uvMin = texOffset / texSheetSize;
        glm::vec2 uvMax = (texOffset + texSize) / texSheetSize;
        shader->setVec2("uvMin", uvMin);
        shader->setVec2("uvMax", uvMax);
    }
    else {
        shader->setVec2("uvMin", glm::vec2(0.0f, 0.0f));
        shader->setVec2("uvMax", glm::vec2(1.0f, 1.0f));
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    shader->setInt("image", 0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

GLuint Sprite::loadTexture(const std::string& path) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
    if (data) {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cerr << "Failed to load texture: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
