cmake_minimum_required(VERSION 3.16)
project(sunnyland_v2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)

add_executable(sunnyland_v2
  src/app/main.cpp

  src/platform/Window.cpp
  src/platform/Input.cpp
  src/platform/Time.cpp
  src/platform/GameLoop.cpp

  src/assets/Texture.cpp
  src/assets/AssetManager.cpp

  src/render/ShaderProgram.cpp
  src/render/SpriteMesh.cpp
  src/render/Renderer.cpp

  src/ecs/World.cpp
  src/ecs/SystemScheduler.cpp

  src/gameplay/systems/PlayerInputSystem.cpp
  src/gameplay/systems/CursorFollowSystem.cpp
  src/gameplay/systems/PixelScaleSystem.cpp
  src/gameplay/systems/MovementSystem.cpp
  src/gameplay/systems/BounceSystem.cpp
  src/gameplay/systems/AnimationSystem.cpp
  src/gameplay/systems/SpriteSyncSystem.cpp
  src/gameplay/systems/RenderSpritesSystem.cpp
  src/gameplay/SunnyLandScene.cpp
)

target_include_directories(sunnyland_v2 PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

target_link_libraries(sunnyland_v2 PRIVATE
  glfw
  glad::glad
  glm::glm
)

if(APPLE)
  target_compile_definitions(sunnyland_v2 PRIVATE GL_SILENCE_DEPRECATION)
endif()

if(WIN32)
  target_link_libraries(sunnyland_v2 PRIVATE opengl32)
endif()

add_custom_command(TARGET sunnyland_v2 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:sunnyland_v2>/assets
  COMMENT "Copy assets next to sunnyland_v2"
)
