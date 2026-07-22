#pragma once

struct GLFWwindow;

namespace platform {

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void pollEvents();
    void swapBuffers();
    bool shouldClose() const;

    GLFWwindow* handle() const { return handle_; }
    int framebufferWidth() const { return framebufferWidth_; }
    int framebufferHeight() const { return framebufferHeight_; }
    float pixelX() const { return pixelX_; }
    float pixelY() const { return pixelY_; }

    void toggleFullscreen();

private:
    static void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods);
    static void framebufferSizeCallback(GLFWwindow* win, int width, int height);
    void onFramebufferResize(int width, int height);

    GLFWwindow* handle_ = nullptr;
    int windowWidth_ = 0;
    int windowHeight_ = 0;
    int framebufferWidth_ = 1;
    int framebufferHeight_ = 1;
    bool windowed_ = true;
    int windowedX_ = 0;
    int windowedY_ = 0;
    int windowedWidth_ = 0;
    int windowedHeight_ = 0;
    float pixelX_ = 2.0f / 640.0f;
    float pixelY_ = 2.0f / 420.0f;
};

} // namespace platform
