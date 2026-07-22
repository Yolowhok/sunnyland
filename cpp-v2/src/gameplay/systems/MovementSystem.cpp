#include "gameplay/systems/MovementSystem.hpp"
#include "gameplay/Components.hpp"

namespace gameplay {

void MovementSystem::update(ecs::World& world, ecs::FrameContext& ctx) {
    world.components<Velocity>().forEach([&](ecs::Entity entity, Velocity& velocity) {
        auto* transform = world.tryGet<Transform>(entity);
        if (!transform) {
            return;
        }
        transform->position.x += velocity.vx * ctx.deltaSeconds;
        transform->position.y += velocity.vy * ctx.deltaSeconds;
    });
}

} // namespace gameplay
