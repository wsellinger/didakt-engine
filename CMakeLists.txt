cmake_minimum_required(VERSION 3.20)
project(Didakt VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#=== Fetch

include(FetchContent)

#SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-2.30.3
)
FetchContent_MakeAvailable(SDL2)

#SDL2_image
FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG        release-2.8.2
)
set(SDL2IMAGE_INSTALL OFF)
FetchContent_MakeAvailable(SDL2_image)

#nlohmann_json
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

#EnTT
FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG        v3.13.2
)
FetchContent_MakeAvailable(entt)

#GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
set(GLM_BUILD_LIBRARY OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glm)

#==========
#=== Engine

#=== Executable

add_executable(Didakt src/main.cpp 
    #Core
    "src/core/Engine.h" "src/core/Engine.cpp" 
    "src/core/Window.h" "src/core/Window.cpp" 
    "src/core/Config.h" "src/core/Config.cpp" 
    "src/core/LogLevel.h"
    "src/core/Logger.h" "src/core/Logger.cpp"

    #Providers
    "src/providers/interfaces/IAssetProvider.h"
    "src/providers/interfaces/IInputProvider.h"
    "src/providers/interfaces/ILoggingProvider.h"
    "src/providers/interfaces/IRenderProvider.h"
    "src/providers/interfaces/ISystemTimerProvider.h"

    "src/providers/sdl/SDLAssetProvider.h" "src/providers/sdl/SDLAssetProvider.cpp"
    "src/providers/sdl/SDLInputProvider.h" "src/providers/sdl/SDLInputProvider.cpp"
    "src/providers/sdl/SDLRenderProvider.h" "src/providers/sdl/SDLRenderProvider.cpp"
    "src/providers/sdl/SDLLoggingProvider.h" "src/providers/sdl/SDLLoggingProvider.cpp"
    "src/providers/sdl/SDLSystemTimerProvider.h" "src/providers/sdl/SDLSystemTimerProvider.cpp"

    #Input
    "src/input/InputManager.h" "src/input/InputManager.cpp"
    "src/input/Key.h" 
    "src/input/MouseButton.h"

    #Render
    "src/render/AssetManager.h" "src/render/AssetManager.cpp"            
    "src/render/Camera.h" 
    "src/render/RenderColor.h"
    "src/render/RenderRect.h"
    "src/render/RenderMath.h"   "src/render/RenderMath.cpp"
    "src/render/TextureHandle.h"

    #Entity Component System
    "src/ecs/RegistryManager.h"    "src/ecs/RegistryManager.cpp"

    #-Components
    "src/ecs/components/BoxColliderComponent.h"
    "src/ecs/components/RenderComponent.h"
    "src/ecs/components/RigidBodyComponent.h"
    "src/ecs/components/SpriteSheetComponent.h"
    "src/ecs/components/TilemapComponent.h"
    "src/ecs/components/TransformComponent.h"

    #-Systems
    "src/ecs/systems/MovementSystem.h"  "src/ecs/systems/MovementSystem.cpp"
    "src/ecs/systems/RenderSystem.h"    "src/ecs/systems/RenderSystem.cpp"
 )

#=== Libraries

target_link_libraries(Didakt PRIVATE 
    SDL2::SDL2 
    SDL2::SDL2main
    SDL2_image::SDL2_image
    nlohmann_json::nlohmann_json
    EnTT::EnTT
    glm::glm
)

#=== Post Build

#SDL2 DLL
add_custom_command(TARGET Didakt POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:Didakt>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2_image::SDL2_image>
        $<TARGET_FILE_DIR:Didakt>
)

#config.json

add_custom_target(CopyConfig ALL
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/config/config.json
        $<TARGET_FILE_DIR:Didakt>/config/config.json
)

add_dependencies(Didakt CopyConfig)

#=========
#=== Tests

#=== Fetch

#Catch2
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.7.1
)
FetchContent_MakeAvailable(Catch2)
enable_testing()

include(CTest)
include(Catch)

#=== Executable

add_executable(DidaktTests
    #Tests
    "tests/core/ConfigTests.cpp"
    "tests/core/LoggerTests.cpp"

    "tests/ecs/MovementSystemTests.cpp"
    "tests/ecs/RenderSystemTests.cpp"

    "tests/input/InputManagerTests.cpp"

    "tests/render/AssetManagerTests.cpp"
    "tests/render/CameraTests.cpp"
    "tests/render/RenderMathTests.cpp"

    #Source    
    "src/core/Config.cpp"
    "src/core/Logger.cpp"

    "src/ecs/systems/MovementSystem.cpp"
    "src/ecs/systems/RenderSystem.cpp"

    "src/input/InputManager.cpp"

    "src/render/AssetManager.cpp"
    "src/render/RenderMath.cpp"

)

#=== Include Directories

target_include_directories(DidaktTests PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
)

#=== Libraries

target_link_libraries(DidaktTests PRIVATE 
    Catch2::Catch2WithMain
    SDL2::SDL2 
    nlohmann_json::nlohmann_json
    EnTT::EnTT
    glm::glm
)

#=== Post Build

add_custom_command(TARGET DidaktTests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Catch2::Catch2>
        $<TARGET_FILE_DIR:DidaktTests>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Catch2::Catch2WithMain>
        $<TARGET_FILE_DIR:DidaktTests>
)

catch_discover_tests(DidaktTests)