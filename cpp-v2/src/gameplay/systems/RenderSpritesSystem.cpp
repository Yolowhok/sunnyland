#include "gameplay/systems/RenderSpritesSystem.hpp"
#include "gameplay/Components.hpp"
#include "render/Renderer.hpp"

namespace gameplay {

void RenderSpritesSystem::update(ecs::World& world, ecs::FrameContext& ctx) {
    if (!ctx.renderer) {
        return;
    }
    ctx.renderer->beginFrame();
    world.components<Sprite>().forEach([&](ecs::Entity entity, Sprite& sprite) {
        (void)entity;
        if (!sprite.texture || !sprite.mesh) {
            return;
        }
        ctx.renderer->draw(*sprite.mesh, *sprite.texture);
    });
    ctx.renderer->endFrame();
}

} // namespace gameplay
