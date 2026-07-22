#pragma once

#include "ecs/System.hpp"

namespace gameplay {

class CursorFollowSystem : public ecs::ISystem {
public:
    ecs::SystemPhase phase() const override { return ecs::SystemPhase::Input; }
    void update(ecs::World& world, ecs::FrameContext& ctx) override;
};

} // namespace gameplay
