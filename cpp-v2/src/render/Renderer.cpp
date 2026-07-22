#include "render/Renderer.hpp"
#include "render/SpriteMesh.hpp"

#include <glad/glad.h>

namespace render {

Renderer::Renderer() : shader_(std::make_unique<ShaderProgram>(ShaderProgram::texturedQuad())) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void Renderer::beginFrame() {
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    shader_->bind();
}

void Renderer::endFrame() {
    shader_->unbind();
}

void Renderer::draw(const SpriteMesh& mesh, const assets::Texture& texture) {
    mesh.draw(texture);
}

} // namespace render
