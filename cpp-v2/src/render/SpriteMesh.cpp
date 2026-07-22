#include "render/SpriteMesh.hpp"
#include "assets/Texture.hpp"

#include <glad/glad.h>

namespace render {

SpriteMesh::SpriteMesh() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          reinterpret_cast<void*>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    uploadVertices();
}

SpriteMesh::~SpriteMesh() {
    if (vbo_ != 0) {
        glDeleteBuffers(1, &vbo_);
        vbo_ = 0;
    }
    if (vao_ != 0) {
        glDeleteVertexArrays(1, &vao_);
        vao_ = 0;
    }
}

void SpriteMesh::setTransform(float x, float y, float sizeX, float sizeY) {
    position_ = Vec2{x, y};
    size_ = Vec2{sizeX, sizeY};
    uploadVertices();
}

void SpriteMesh::draw(const assets::Texture& texture) const {
    glBindVertexArray(vao_);
    glBindTexture(GL_TEXTURE_2D, texture.id());
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

void SpriteMesh::uploadVertices() {
    const float hx = size_.x * 0.5f;
    const float hy = size_.y * 0.5f;
    float vertices[16] = {
        position_.x - hx, position_.y + hy, 0.0f, 1.0f,
        position_.x - hx, position_.y - hy, 0.0f, 0.0f,
        position_.x + hx, position_.y - hy, 1.0f, 0.0f,
        position_.x + hx, position_.y + hy, 1.0f, 1.0f,
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace render
