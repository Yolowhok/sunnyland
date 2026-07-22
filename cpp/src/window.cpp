#include "window.h"

Window::Window(int width, int height, const char* title){
    this->width = width;
    this->height = height;
    this->title = title;
    this->isWindowed = true;
    this->window = nullptr;
    std::cout << "window object created!" << std::endl;
    
}

void Window::windowInit() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    this->window = glfwCreateWindow(width, height, title, NULL, NULL);

    

    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        return;
    }
    glfwSetFramebufferSizeCallback(window, Window::framebufferSizeCallback);
// Привязываем указатель на текущий объект Window к GLFW-окну
    glfwSetWindowUserPointer(window, this);
// Устанавливаем статический колбэк
    glfwSetKeyCallback(window, Window::keyboardCallback);
    glfwGetWindowPos(window, &windowedX, &windowedY);
    glfwGetWindowSize(window, &windowedWidth, &windowedHeight);
}


void Window::switchWindowMode(){
    if (isWindowed){
        glfwGetWindowPos(window, &windowedX, &windowedY);
        glfwGetWindowSize(window, &windowedWidth, &windowedHeight);
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        isWindowed = false;
        std::cout << "Fullscreen mode toggled!" << std::endl;
    }
    else{
        glfwSetWindowMonitor(window, nullptr, windowedX, windowedY, windowedWidth, windowedHeight, 0);
        isWindowed = true;
        std::cout << "Window mode toggled!" << std::endl;
    }
}

void Window::keyboardCallback(GLFWwindow* glfwWin, int key, int scancode, int action, int mods) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(glfwWin));
    if (!win) return;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(glfwWin, GLFW_TRUE);
    }
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        win->switchWindowMode();
    }
}


void Window::framebufferSizeCallback(GLFWwindow* glfwWin, int newWidth, int newHeight) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(glfwWin));
    if (win) {
        win->width = newWidth;
        win->height = newHeight;
        glViewport(0, 0, newWidth, newHeight);
        win->pixel_x = 2.0f / newWidth;//Для разрешения пикселей.
        win->pixel_y = 2.0f / newHeight;//Для разрешения пикселей.
    }
}


Window::~Window() {
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}