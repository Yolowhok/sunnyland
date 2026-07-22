#pragma once

#include "assets/Texture.hpp"
#include "render/SpriteMesh.hpp"
#include "render/Vec2.hpp"

#include <memory>
#include <vector>

namespace gameplay {

struct Transform {
    render::Vec2 position{};
    render::Vec2 size{1.0f, 1.0f};
};

struct Velocity {
    float vx = 0.0f;
    float vy = 0.0f;
};

struct Sprite {
    assets::Texture* texture = nullptr;
    std::unique_ptr<render::SpriteMesh> mesh;
};

struct Animation {
    std::vector<assets::Texture*> frames;
    float frameDuration = 1.0f / 60.0f;
    float elapsed = 0.0f;
    int index = 0;
};

struct PlayerControl {
    float speed = 0.0f;
};

struct Bounce {
    float limit = 0.6f;
};

struct CursorFollow {};

struct PixelSize {
    float basePixels = 64.0f;
    float scale = 1.0f;
};

} // namespace gameplay
