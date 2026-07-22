#pragma once

struct GLFWwindow;

namespace platform {

class Window;

class Input {
public:
    explicit Input(Window& window);

    bool isKeyDown(int key) const;
    void setCursorHidden(bool hidden);
    void readMouse(double& outX, double& outY) const;

private:
    GLFWwindow* handle_;
};

} // namespace platform
