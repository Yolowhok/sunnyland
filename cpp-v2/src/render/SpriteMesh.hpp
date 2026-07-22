#pragma once

#include "render/Vec2.hpp"

namespace assets {
class Texture;
}

namespace render {

class SpriteMesh {
public:
    SpriteMesh();
    ~SpriteMesh();

    SpriteMesh(const SpriteMesh&) = delete;
    SpriteMesh& operator=(const SpriteMesh&) = delete;

    void setTransform(float x, float y, float sizeX, float sizeY);
    void draw(const assets::Texture& texture) const;

    const Vec2& position() const { return position_; }
    const Vec2& size() const { return size_; }

private:
    void uploadVertices();

    unsigned int vao_ = 0;
    unsigned int vbo_ = 0;
    Vec2 position_{};
    Vec2 size_{1.0f, 1.0f};
};

} // namespace render
