#pragma once
#include <GL/glew.h>
#include <string>

GLuint loadShader(const char* vertexPath, const char* fragmentPath);
GLuint loadTexture(const char* path);
