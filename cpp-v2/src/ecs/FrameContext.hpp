#pragma once

namespace platform {
class Window;
class Input;
}

namespace assets {
class AssetManager;
}

namespace render {
class Renderer;
}

namespace ecs {

struct FrameContext {
    float deltaSeconds = 0.0f;
    platform::Window* window = nullptr;
    platform::Input* input = nullptr;
    assets::AssetManager* assets = nullptr;
    render::Renderer* renderer = nullptr;
};

} // namespace ecs
