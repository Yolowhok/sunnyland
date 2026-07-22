#pragma once

#include "assets/AssetManager.hpp"
#include "ecs/SystemScheduler.hpp"
#include "ecs/World.hpp"
#include "platform/Input.hpp"
#include "platform/Window.hpp"
#include "render/Renderer.hpp"

namespace gameplay {

class SunnyLandScene {
public:
    SunnyLandScene(platform::Window& window, platform::Input& input, assets::AssetManager& assets,
                   render::Renderer& renderer);

    void onFrame(float deltaSeconds);

private:
    void spawnEntities();
    void registerSystems();

    platform::Window& window_;
    platform::Input& input_;
    assets::AssetManager& assets_;
    render::Renderer& renderer_;

    ecs::World world_;
    ecs::SystemScheduler scheduler_;
};

} // namespace gameplay
