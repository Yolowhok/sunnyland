#include "gameplay/systems/PixelScaleSystem.hpp"
#include "gameplay/Components.hpp"
#include "platform/Window.hpp"

namespace gameplay {

void PixelScaleSystem::update(ecs::World& world, ecs::FrameContext& ctx) {
    if (!ctx.window) {
        return;
    }
    const float px = ctx.window->pixelX();
    const float py = ctx.window->pixelY();
    world.components<PixelSize>().forEach([&](ecs::Entity entity, PixelSize& pixel) {
        auto* transform = world.tryGet<Transform>(entity);
        if (!transform) {
            return;
        }
        transform->size.x = pixel.basePixels * pixel.scale * px;
        transform->size.y = pixel.basePixels * pixel.scale * py;
    });
}

} // namespace gameplay
