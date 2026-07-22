#include "platform/Input.hpp"
#include "platform/Window.hpp"

#include <GLFW/glfw3.h>

namespace platform {

Input::Input(Window& window) : handle_(window.handle()) {}

bool Input::isKeyDown(int key) const {
    return glfwGetKey(handle_, key) == GLFW_PRESS;
}

void Input::setCursorHidden(bool hidden) {
    glfwSetInputMode(handle_, GLFW_CURSOR, hidden ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
}

void Input::readMouse(double& outX, double& outY) const {
    glfwGetCursorPos(handle_, &outX, &outY);
}

} // namespace platform
