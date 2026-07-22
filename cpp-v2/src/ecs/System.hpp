#pragma once

#include "ecs/FrameContext.hpp"
#include "ecs/World.hpp"

namespace ecs {

enum class SystemPhase {
    Input,
    Update,
    Sync,
    Render
};

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual SystemPhase phase() const = 0;
    virtual void update(World& world, FrameContext& ctx) = 0;
};

} // namespace ecs
