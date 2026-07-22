#include "assets/Texture.hpp"

#include <glad/glad.h>

namespace assets {

Texture::Texture(unsigned int id, int width, int height)
    : id_(id), width_(width), height_(height) {}

Texture::~Texture() {
    if (id_ != 0) {
        glDeleteTextures(1, &id_);
        id_ = 0;
    }
}

Texture::Texture(Texture&& other) noexcept
    : id_(other.id_), width_(other.width_), height_(other.height_) {
    other.id_ = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (id_ != 0) {
            glDeleteTextures(1, &id_);
        }
        id_ = other.id_;
        width_ = other.width_;
        height_ = other.height_;
        other.id_ = 0;
    }
    return *this;
}

} // namespace assets
