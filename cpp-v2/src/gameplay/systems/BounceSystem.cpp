#include "gameplay/systems/BounceSystem.hpp"
#include "gameplay/Components.hpp"

#include <cmath>
#include <iostream>

namespace gameplay {

void BounceSystem::update(ecs::World& world, ecs::FrameContext& ctx) {
    (void)ctx;
    world.components<Bounce>().forEach([&](ecs::Entity entity, Bounce& bounce) {
        auto* transform = world.tryGet<Transform>(entity);
        auto* velocity = world.tryGet<Velocity>(entity);
        if (!transform || !velocity) {
            return;
        }
        if (std::fabs(transform->position.x) >= bounce.limit) {
            std::cout << "hit!" << std::endl;
            velocity->vx = -velocity->vx;
            transform->position.x = std::copysign(bounce.limit, transform->position.x);
        }
        if (std::fabs(transform->position.y) >= bounce.limit) {
            std::cout << "hit!" << std::endl;
            velocity->vy = -velocity->vy;
            transform->position.y = std::copysign(bounce.limit, transform->position.y);
        }
    });
}

} // namespace gameplay
