#pragma once

#include "ecs/System.hpp"

namespace gameplay {

class MovementSystem : public ecs::ISystem {
public:
    ecs::SystemPhase phase() const override { return ecs::SystemPhase::Update; }
    void update(ecs::World& world, ecs::FrameContext& ctx) override;
};

} // namespace gameplay
