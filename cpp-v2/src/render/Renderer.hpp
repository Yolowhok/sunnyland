#pragma once

#include "render/ShaderProgram.hpp"

#include <memory>

namespace assets {
class Texture;
}

namespace render {

class SpriteMesh;

class Renderer {
public:
    Renderer();
    ~Renderer() = default;

    void beginFrame();
    void endFrame();
    void draw(const SpriteMesh& mesh, const assets::Texture& texture);

private:
    std::unique_ptr<ShaderProgram> shader_;
};

} // namespace render
