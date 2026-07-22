#include "gameplay/systems/PlayerInputSystem.hpp"
#include "gameplay/Components.hpp"
#include "platform/Input.hpp"

#include <GLFW/glfw3.h>

namespace gameplay {

void PlayerInputSystem::update(ecs::World& world, ecs::FrameContext& ctx) {
    if (!ctx.input) {
        return;
    }
    world.components<PlayerControl>().forEach([&](ecs::Entity entity, PlayerControl& control) {
        auto* transform = world.tryGet<Transform>(entity);
        if (!transform) {
            return;
        }
        const float step = control.speed * ctx.deltaSeconds;
        if (ctx.input->isKeyDown(GLFW_KEY_W)) {
            transform->position.y += step;
        }
        if (ctx.input->isKeyDown(GLFW_KEY_S)) {
            transform->position.y -= step;
        }
        if (ctx.input->isKeyDown(GLFW_KEY_A)) {
            transform->position.x -= step;
        }
        if (ctx.input->isKeyDown(GLFW_KEY_D)) {
            transform->position.x += step;
        }
    });
}

} // namespace gameplay
