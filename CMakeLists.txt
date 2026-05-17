cmake_minimum_required(VERSION 3.20)
project(Didakt VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Fetch

include(FetchContent)

#SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-2.30.3
)
FetchContent_MakeAvailable(SDL2)

#nlohmann_json
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(Didakt src/main.cpp "src/Window.h" "src/Window.cpp" "src/Engine.h" "src/Engine.cpp" "src/Config.h" "src/Config.cpp")

target_link_libraries(Didakt PRIVATE 
    SDL2::SDL2 
    SDL2::SDL2main
    nlohmann_json::nlohmann_json
)

#Copy Files to Build Dir

#SDL2 DLL
add_custom_command(TARGET Didakt POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:Didakt>
)

#config.json

add_custom_target(CopyConfig ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/config/config.json
        $<TARGET_FILE_DIR:Didakt>/config/config.json
    DEPENDS ${CMAKE_SOURCE_DIR}/config/config.json
)

add_dependencies(Didakt CopyConfig)