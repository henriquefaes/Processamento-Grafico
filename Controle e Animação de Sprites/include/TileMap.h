
#pragma once
#include <glad/glad.h>
#include <vector>

class TileMap {
public:
    TileMap(const char* tilesetPath, int tileCols, int tileRows, int mapWidth, int mapHeight);
    ~TileMap();
    void draw();

private:
    GLuint VAO, VBO, EBO, textureID, shaderProgram;
    int tilesPerRow, tilesPerCol;
    int mapW, mapH;
    std::vector<int> tileData;

    void setupShader();
    void setupMesh();
    void loadTexture(const char* path);
    void generateMap();
};
