#pragma once

#include "ecs/System.hpp"

namespace gameplay {

class SpriteSyncSystem : public ecs::ISystem {
public:
    ecs::SystemPhase phase() const override { return ecs::SystemPhase::Sync; }
    void update(ecs::World& world, ecs::FrameContext& ctx) override;
};

} // namespace gameplay
