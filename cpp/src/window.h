#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window{
private:
    bool isWindowed;
    int height;
    int width;
    
    const char* title;
    GLFWwindow* window;
    int windowedX, windowedY, windowedWidth, windowedHeight;
    
public:
    float pixel_x = 2.0f/ 640;
    float pixel_y = 2.0f/ 420;
    Window(int width, int height, const char* title);
    void windowInit();
    void switchWindowMode();
    GLFWwindow* getWindow() const { return window; }
    static void keyboardCallback(GLFWwindow* glfwWin, int key, int scancode, int action, int mods);
    static void framebufferSizeCallback(GLFWwindow* glfwWin, int newWidth, int newHeight);
    ~Window();
    void pixelResolution(int width, int height);
    bool getPress();
};

#endif