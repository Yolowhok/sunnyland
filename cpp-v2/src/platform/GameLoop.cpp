#include "platform/GameLoop.hpp"
#include "platform/Time.hpp"
#include "platform/Window.hpp"

namespace platform {

GameLoop::GameLoop(Window& window, Time& time) : window_(window), time_(time) {}

void GameLoop::run(const FrameFn& onFrame) {
    while (!window_.shouldClose()) {
        const float dt = time_.tick();
        window_.pollEvents();
        onFrame(dt);
        window_.swapBuffers();
    }
}

} // namespace platform
