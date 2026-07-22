#include "assets/AssetManager.hpp"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <stdexcept>

namespace assets {

AssetManager::AssetManager(std::string rootDirectory) : root_(std::move(rootDirectory)) {}

Texture& AssetManager::getTexture(const std::string& relativePath) {
    auto it = cache_.find(relativePath);
    if (it != cache_.end()) {
        return *it->second;
    }
    const std::string fullPath = root_ + "/" + relativePath;
    auto texture = std::make_unique<Texture>(loadTexture(fullPath));
    Texture& ref = *texture;
    cache_.emplace(relativePath, std::move(texture));
    return ref;
}

Texture AssetManager::loadTexture(const std::string& fullPath) {
    unsigned int textureId = 0;
    glGenTextures(1, &textureId);

    int width = 0, height = 0, channels = 0;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &channels, 0);
    if (!data) {
        glDeleteTextures(1, &textureId);
        throw std::runtime_error(std::string("Failed to load texture: ") + fullPath + " - " +
                                 stbi_failure_reason());
    }

    GLenum format = GL_RGBA;
    if (channels == 1) {
        format = GL_RED;
    } else if (channels == 3) {
        format = GL_RGB;
    } else if (channels == 4) {
        format = GL_RGBA;
    } else {
        stbi_image_free(data);
        glDeleteTextures(1, &textureId);
        throw std::runtime_error("Unsupported channel count for " + fullPath);
    }

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(data);

    std::cout << "Texture loaded: " << fullPath << " (" << width << "x" << height << ", "
              << channels << " channels)" << std::endl;

    return Texture(textureId, width, height);
}

} // namespace assets
