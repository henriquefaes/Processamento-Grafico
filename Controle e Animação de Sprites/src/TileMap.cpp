#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "TileMap.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

const char* tileVertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

const char* tileFragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D tileset;

void main() {
    FragColor = texture(tileset, TexCoord);
}
)";

TileMap::TileMap(const char* tilesetPath, int tileCols, int tileRows, int mapWidth, int mapHeight)
    : tilesPerRow(tileCols), tilesPerCol(tileRows), mapW(mapWidth), mapH(mapHeight) {
    setupShader();
    loadTexture(tilesetPath);
    generateMap();
    setupMesh();
}

TileMap::~TileMap() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void TileMap::setupShader() {
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &tileVertexShaderSrc, NULL);
    glCompileShader(vShader);

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &tileFragmentShaderSrc, NULL);
    glCompileShader(fShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

void TileMap::loadTexture(const char* path) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 4);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else {
        std::cout << "Failed to load tileset" << std::endl;
    }
    stbi_image_free(data);
}

void TileMap::generateMap() {
    srand((unsigned int)time(0));
    tileData.resize(mapW * mapH);
    for (int i = 0; i < mapW * mapH; ++i) {
        tileData[i] = rand() % (tilesPerRow * tilesPerCol);
    }
}

void TileMap::setupMesh() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    float isoW = 0.12f;
    float isoH = 0.06f;

    float ts = 1.0f / tilesPerRow;
    float tt = 1.0f / tilesPerCol;

    int idx = 0;
    for (int y = 0; y < mapH; ++y) {
        for (int x = 0; x < mapW; ++x) {
            int tileIndex = tileData[y * mapW + x];
            int tx = tileIndex % tilesPerRow;
            int ty = tileIndex / tilesPerRow;

            float s = tx * ts;
            float t = ty * tt;

            float cx = (x - y) * isoW;
            float cy = (x + y) * isoH;

            float quad[] = {
                cx - isoW, cy + isoH, 0.0f, s, t + tt,
                cx - isoW, cy - isoH, 0.0f, s, t,
                cx + isoW, cy + isoH, 0.0f, s + ts, t + tt,
                cx + isoW, cy - isoH, 0.0f, s + ts, t
            };

            for (int i = 0; i < 20; ++i)
                vertices.push_back(quad[i]);

            int base = idx * 4;
            indices.push_back(base);
            indices.push_back(base + 1);
            indices.push_back(base + 2);
            indices.push_back(base + 1);
            indices.push_back(base + 2);
            indices.push_back(base + 3);
            idx++;
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void TileMap::draw() {
    glUseProgram(shaderProgram);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, mapW * mapH * 6, GL_UNSIGNED_INT, 0);
}
