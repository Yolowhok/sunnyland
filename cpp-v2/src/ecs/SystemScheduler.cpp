#include "ecs/SystemScheduler.hpp"

namespace ecs {

void SystemScheduler::add(std::unique_ptr<ISystem> system) {
    systems_.push_back(std::move(system));
}

void SystemScheduler::runPhase(SystemPhase phase, World& world, FrameContext& ctx) {
    for (auto& system : systems_) {
        if (system->phase() == phase) {
            system->update(world, ctx);
        }
    }
}

void SystemScheduler::runFrame(World& world, FrameContext& ctx) {
    runPhase(SystemPhase::Input, world, ctx);
    runPhase(SystemPhase::Update, world, ctx);
    runPhase(SystemPhase::Sync, world, ctx);
    runPhase(SystemPhase::Render, world, ctx);
}

} // namespace ecs
