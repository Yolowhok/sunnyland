#include "gameplay/SunnyLandScene.hpp"
#include "gameplay/Components.hpp"
#include "gameplay/systems/AnimationSystem.hpp"
#include "gameplay/systems/BounceSystem.hpp"
#include "gameplay/systems/CursorFollowSystem.hpp"
#include "gameplay/systems/MovementSystem.hpp"
#include "gameplay/systems/PixelScaleSystem.hpp"
#include "gameplay/systems/PlayerInputSystem.hpp"
#include "gameplay/systems/RenderSpritesSystem.hpp"
#include "gameplay/systems/SpriteSyncSystem.hpp"
#include "ecs/FrameContext.hpp"
#include "render/SpriteMesh.hpp"

#include <memory>
#include <string>

namespace gameplay {

namespace {
constexpr float kPlayerSpeed = 0.003f * 60.0f;
constexpr float kBallSpeedX = 0.01f * 60.0f;
constexpr float kBallSpeedY = 0.005f * 60.0f;
constexpr float kBounceLimit = 0.6f;
constexpr float kAnimationFps = 60.0f;
} // namespace

SunnyLandScene::SunnyLandScene(platform::Window& window, platform::Input& input,
                               assets::AssetManager& assets, render::Renderer& renderer)
    : window_(window), input_(input), assets_(assets), renderer_(renderer) {
    input_.setCursorHidden(true);
    registerSystems();
    spawnEntities();
}

void SunnyLandScene::registerSystems() {
    // Input
    scheduler_.add(std::make_unique<PlayerInputSystem>());
    scheduler_.add(std::make_unique<CursorFollowSystem>());
    // Update
    scheduler_.add(std::make_unique<PixelScaleSystem>());
    scheduler_.add(std::make_unique<MovementSystem>());
    scheduler_.add(std::make_unique<BounceSystem>());
    scheduler_.add(std::make_unique<AnimationSystem>());
    // Sync
    scheduler_.add(std::make_unique<SpriteSyncSystem>());
    // Render
    scheduler_.add(std::make_unique<RenderSpritesSystem>());
}

void SunnyLandScene::spawnEntities() {
    // Player
    {
        const ecs::Entity player = world_.createEntity();
        world_.add<Transform>(player);
        world_.add<PlayerControl>(player, PlayerControl{kPlayerSpeed});
        world_.add<PixelSize>(player, PixelSize{64.0f, 4.0f});

        Animation animation;
        animation.frameDuration = 1.0f / kAnimationFps;
        animation.frames.reserve(40);
        for (int i = 1; i <= 40; ++i) {
            const std::string path = "character/idle/bb" + std::to_string(i) + ".png";
            animation.frames.push_back(&assets_.getTexture(path));
        }

        Sprite sprite;
        sprite.mesh = std::make_unique<render::SpriteMesh>();
        sprite.texture = animation.frames.front();
        world_.add<Sprite>(player, std::move(sprite));
        world_.add<Animation>(player, std::move(animation));
    }

    // Ball
    {
        const ecs::Entity ball = world_.createEntity();
        world_.add<Transform>(ball);
        world_.add<Velocity>(ball, Velocity{kBallSpeedX, kBallSpeedY});
        world_.add<Bounce>(ball, Bounce{kBounceLimit});
        world_.add<PixelSize>(ball, PixelSize{64.0f, 1.0f});

        Sprite sprite;
        sprite.mesh = std::make_unique<render::SpriteMesh>();
        sprite.texture = &assets_.getTexture("ball.png");
        world_.add<Sprite>(ball, std::move(sprite));
    }

    // Cursor
    {
        const ecs::Entity cursor = world_.createEntity();
        world_.add<Transform>(cursor);
        world_.add<CursorFollow>(cursor);
        world_.add<PixelSize>(cursor, PixelSize{64.0f, 4.0f});

        Sprite sprite;
        sprite.mesh = std::make_unique<render::SpriteMesh>();
        sprite.texture = &assets_.getTexture("cursor.png");
        world_.add<Sprite>(cursor, std::move(sprite));
    }
}

void SunnyLandScene::onFrame(float deltaSeconds) {
    ecs::FrameContext ctx;
    ctx.deltaSeconds = deltaSeconds;
    ctx.window = &window_;
    ctx.input = &input_;
    ctx.assets = &assets_;
    ctx.renderer = &renderer_;
    scheduler_.runFrame(world_, ctx);
}

} // namespace gameplay
