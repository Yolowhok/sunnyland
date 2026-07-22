#include "gameplay/systems/AnimationSystem.hpp"
#include "gameplay/Components.hpp"

namespace gameplay {

void AnimationSystem::update(ecs::World& world, ecs::FrameContext& ctx) {
    world.components<Animation>().forEach([&](ecs::Entity entity, Animation& animation) {
        if (animation.frames.empty()) {
            return;
        }
        animation.elapsed += ctx.deltaSeconds;
        while (animation.elapsed >= animation.frameDuration) {
            animation.elapsed -= animation.frameDuration;
            animation.index = (animation.index + 1) % static_cast<int>(animation.frames.size());
        }
        if (auto* sprite = world.tryGet<Sprite>(entity)) {
            sprite->texture = animation.frames[static_cast<std::size_t>(animation.index)];
        }
    });
}

} // namespace gameplay
