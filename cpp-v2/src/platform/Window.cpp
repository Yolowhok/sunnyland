#include "platform/Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>

namespace platform {

Window::Window(int width, int height, const char* title)
    : windowWidth_(width), windowHeight_(height) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    handle_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!handle_) {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSetWindowUserPointer(handle_, this);
    glfwSetKeyCallback(handle_, Window::keyCallback);
    glfwSetFramebufferSizeCallback(handle_, Window::framebufferSizeCallback);

    glfwGetWindowPos(handle_, &windowedX_, &windowedY_);
    glfwGetWindowSize(handle_, &windowedWidth_, &windowedHeight_);

    glfwMakeContextCurrent(handle_);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    int fbW = 0, fbH = 0;
    glfwGetFramebufferSize(handle_, &fbW, &fbH);
    onFramebufferResize(fbW, fbH);
}

Window::~Window() {
    if (handle_) {
        glfwDestroyWindow(handle_);
        handle_ = nullptr;
    }
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::swapBuffers() {
    glfwSwapBuffers(handle_);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(handle_) == GLFW_TRUE;
}

void Window::toggleFullscreen() {
    if (windowed_) {
        glfwGetWindowPos(handle_, &windowedX_, &windowedY_);
        glfwGetWindowSize(handle_, &windowedWidth_, &windowedHeight_);
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(handle_, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        windowed_ = false;
    } else {
        glfwSetWindowMonitor(handle_, nullptr, windowedX_, windowedY_, windowedWidth_, windowedHeight_, 0);
        windowed_ = true;
    }
}

void Window::keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    (void)scancode;
    (void)mods;
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(win));
    if (!self) {
        return;
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        self->toggleFullscreen();
    }
}

void Window::framebufferSizeCallback(GLFWwindow* win, int width, int height) {
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(win));
    if (self) {
        self->onFramebufferResize(width, height);
    }
}

void Window::onFramebufferResize(int width, int height) {
    framebufferWidth_ = width > 0 ? width : 1;
    framebufferHeight_ = height > 0 ? height : 1;
    pixelX_ = 2.0f / static_cast<float>(framebufferWidth_);
    pixelY_ = 2.0f / static_cast<float>(framebufferHeight_);
    glViewport(0, 0, framebufferWidth_, framebufferHeight_);
}

} // namespace platform
