#include "assets/AssetManager.hpp"
#include "gameplay/SunnyLandScene.hpp"
#include "platform/GameLoop.hpp"
#include "platform/Input.hpp"
#include "platform/Time.hpp"
#include "platform/Window.hpp"
#include "render/Renderer.hpp"

#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>

int main() {
    std::cout << "Sunny Land! 0.2 (ECS + domains)" << std::endl;

    glfwSetErrorCallback([](int, const char* description) {
        std::cerr << "GLFW error: " << description << std::endl;
    });

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    try {
        platform::Window window(640, 420, "Sunny land!");
        platform::Input input(window);
        platform::Time time;
        assets::AssetManager assets("assets");
        render::Renderer renderer;
        gameplay::SunnyLandScene scene(window, input, assets, renderer);

        platform::GameLoop loop(window, time);
        loop.run([&](float dt) { scene.onFrame(dt); });
    } catch (const std::exception& ex) {
        std::cerr << "Fatal: " << ex.what() << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwTerminate();
    return 0;
}
