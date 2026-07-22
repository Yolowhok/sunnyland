#pragma once

#include "ecs/System.hpp"

#include <memory>
#include <vector>

namespace ecs {

class SystemScheduler {
public:
    void add(std::unique_ptr<ISystem> system);

    void runPhase(SystemPhase phase, World& world, FrameContext& ctx);
    void runFrame(World& world, FrameContext& ctx);

private:
    std::vector<std::unique_ptr<ISystem>> systems_;
};

} // namespace ecs
