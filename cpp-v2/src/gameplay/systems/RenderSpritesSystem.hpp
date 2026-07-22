#pragma once

#include "ecs/System.hpp"

namespace gameplay {

class RenderSpritesSystem : public ecs::ISystem {
public:
    ecs::SystemPhase phase() const override { return ecs::SystemPhase::Render; }
    void update(ecs::World& world, ecs::FrameContext& ctx) override;
};

} // namespace gameplay
