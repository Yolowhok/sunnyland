#pragma once

#include <functional>

namespace platform {

class Window;
class Time;

class GameLoop {
public:
    using FrameFn = std::function<void(float deltaSeconds)>;

    GameLoop(Window& window, Time& time);

    void run(const FrameFn& onFrame);

private:
    Window& window_;
    Time& time_;
};

} // namespace platform
