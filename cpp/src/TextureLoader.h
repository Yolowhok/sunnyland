// TextureLoader.h
#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <glad/glad.h>

// Загружает текстуру из файла и возвращает OpenGL ID
GLuint loadTexture(const char* filepath);

#endif