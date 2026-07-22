#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

GLuint loadTexture(const char* filepath) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data =stbi_load(filepath, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else{
            std::cerr << "Unsupported channel count: " << nrChannels << std::endl;
            stbi_image_free(data);
            glDeleteTextures(1, &textureID);
            return 0;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

         // Настройки фильтрации и оборачивания
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        std::cout << "Texture loaded: " << filepath 
                  << " (" << width << "x" << height << ", " << nrChannels << " channels)" << std::endl;

        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture: " << filepath 
                  << " - " << stbi_failure_reason() << std::endl;
        glDeleteTextures(1, &textureID);
        return 0;
    }
    return textureID;
}