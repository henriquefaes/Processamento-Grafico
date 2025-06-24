
#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Sprite {
public:
    Sprite(const char* spriteSheetPath, int animations, int frames);
    virtual ~Sprite();
    void setPosition(float x, float y);
    virtual void update(float deltaTime);
    virtual void draw();

public:
    GLuint VAO, VBO, EBO, textureID, shaderProgram;
    int nAnimations, nFrames;
    int currentFrame, currentAnimation;
    float ds, dt;
    float x, y;

    void loadTexture(const char* path);
    void setupMesh();
    void setupShader();
    void updateTexCoords();
};
