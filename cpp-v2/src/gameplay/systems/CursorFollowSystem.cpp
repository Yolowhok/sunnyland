#include "gameplay/systems/CursorFollowSystem.hpp"
#include "gameplay/Components.hpp"
#include "platform/Input.hpp"
#include "platform/Window.hpp"

namespace gameplay {

void CursorFollowSystem::update(ecs::World& world, ecs::FrameContext& ctx) {
    if (!ctx.input || !ctx.window) {
        return;
    }
    double mouseX = 0.0;
    double mouseY = 0.0;
    ctx.input->readMouse(mouseX, mouseY);
    const float ndcX =
        static_cast<float>((2.0 * mouseX) / ctx.window->framebufferWidth() - 1.0);
    const float ndcY =
        static_cast<float>(1.0 - (2.0 * mouseY) / ctx.window->framebufferHeight());

    world.components<CursorFollow>().forEach([&](ecs::Entity entity, CursorFollow&) {
        auto* transform = world.tryGet<Transform>(entity);
        if (transform) {
            transform->position.x = ndcX;
            transform->position.y = ndcY;
        }
    });
}

} // namespace gameplay
