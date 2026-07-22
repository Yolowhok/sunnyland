#include "platform/Time.hpp"

#include <GLFW/glfw3.h>

namespace platform {

Time::Time() : last_(nowSeconds()) {}

double Time::nowSeconds() {
    return glfwGetTime();
}

float Time::tick() {
    const double now = nowSeconds();
    float delta = static_cast<float>(now - last_);
    last_ = now;
    if (delta > maxDelta_) {
        delta = maxDelta_;
    }
    if (delta < 0.0f) {
        delta = 0.0f;
    }
    return delta;
}

} // namespace platform
