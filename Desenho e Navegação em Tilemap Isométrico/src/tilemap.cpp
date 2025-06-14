#include "tilemap.h"
#include "map_data.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const int TILE_WIDTH = 128;
const int TILE_HEIGHT = 64;
const int TILE_COUNT = 7;

GLuint VAO, VBO, EBO;
bool initialized = false;

void initTilemap() {
    float vertices[] = {
        // positions      // tex coords
        0.0f, 0.0f,       0.0f, 0.0f,
        TILE_WIDTH, 0.0f, 1.0f, 0.0f,
        TILE_WIDTH, TILE_HEIGHT, 1.0f, 1.0f,
        0.0f, TILE_HEIGHT, 0.0f, 1.0f
    };
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    initialized = true;
}

void drawTilemap(GLuint shaderProgram, GLuint texture, int selectedRow, int selectedCol) {
    if (!initialized) initTilemap();

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

        glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

        for (int row = 0; row < MAP_ROWS; ++row) {
            for (int col = 0; col < MAP_COLS; ++col) {
                int tileIndex = map[row][col];
                float u = (float)tileIndex / TILE_COUNT;
                float du = 1.0f / TILE_COUNT;

                float screenX = (col - row) * (TILE_WIDTH / 2.0f) + 400;
                float screenY = (col + row) * (TILE_HEIGHT / 2.0f) + 100;

                float texCoords[] = {
                    u,     0.0f,
                    u + du,  0.0f,
                    u + du,  1.0f,
                    u,     1.0f
                };

                glUniform2f(glGetUniformLocation(shaderProgram, "offset"), screenX, screenY);

                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                float vertices[] = {
                    0.0f, 0.0f,         texCoords[0], texCoords[1],
                    TILE_WIDTH, 0.0f,   texCoords[2], texCoords[3],
                    TILE_WIDTH, TILE_HEIGHT, texCoords[4], texCoords[5],
                    0.0f, TILE_HEIGHT,  texCoords[6], texCoords[7]
                };
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }
}
