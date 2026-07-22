#include "gameplay/systems/SpriteSyncSystem.hpp"
#include "gameplay/Components.hpp"

namespace gameplay {

void SpriteSyncSystem::update(ecs::World& world, ecs::FrameContext& ctx) {
    (void)ctx;
    world.components<Sprite>().forEach([&](ecs::Entity entity, Sprite& sprite) {
        auto* transform = world.tryGet<Transform>(entity);
        if (!transform || !sprite.mesh) {
            return;
        }
        sprite.mesh->setTransform(transform->position.x, transform->position.y, transform->size.x,
                                  transform->size.y);
    });
}

} // namespace gameplay
