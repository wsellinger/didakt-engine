option(DIDAKT_USE_SDL "Build Didakt using the SDL backend" ON)

#=== Provider Interfaces (always compiled, backend-agnostic)

set(DIDAKT_PROVIDER_SOURCES
    "src/providers/interfaces/IAssetProvider.h"
    "src/providers/interfaces/IInputProvider.h"
    "src/providers/interfaces/ILoggingProvider.h"
    "src/providers/interfaces/IProviderManager.h"
    "src/providers/interfaces/IRenderProvider.h"
    "src/providers/interfaces/ISystemEventProvider.h"
    "src/providers/interfaces/ISystemTimerProvider.h"
    "src/providers/interfaces/IWindowProvider.h"

    "src/providers/ProviderManagerFactory.h" "src/providers/ProviderManagerFactory.cpp"
)

#=== SDL Backend

if(DIDAKT_USE_SDL)
    list(APPEND DIDAKT_PROVIDER_SOURCES
        "src/providers/sdl/SDLAssetProvider.h"       "src/providers/sdl/SDLAssetProvider.cpp"
        "src/providers/sdl/SDLInputProvider.h"       "src/providers/sdl/SDLInputProvider.cpp"
        "src/providers/sdl/SDLLoggingProvider.h"     "src/providers/sdl/SDLLoggingProvider.cpp"
        "src/providers/sdl/SDLProviderManager.h"     "src/providers/sdl/SDLProviderManager.cpp"
        "src/providers/sdl/SDLRenderProvider.h"      "src/providers/sdl/SDLRenderProvider.cpp"
        "src/providers/sdl/SDLSystemEventProvider.h" "src/providers/sdl/SDLSystemEventProvider.cpp"
        "src/providers/sdl/SDLSystemTimerProvider.h" "src/providers/sdl/SDLSystemTimerProvider.cpp"
        "src/providers/sdl/SDLWindowProvider.h"      "src/providers/sdl/SDLWindowProvider.cpp"
    )
endif()

#=== Target Configuration Helper

function(didakt_configure_provider_target target)
    if(DIDAKT_USE_SDL)
        target_compile_definitions(${target} PRIVATE DIDAKT_USE_SDL)
        target_link_libraries(${target} PRIVATE
            SDL2::SDL2
            SDL2_image::SDL2_image
        )

        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:SDL2::SDL2>
                $<TARGET_FILE_DIR:${target}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:SDL2_image::SDL2_image>
                $<TARGET_FILE_DIR:${target}>
        )
    endif()
endfunction()

function(didakt_link_sdl2main target)
    if(DIDAKT_USE_SDL)
        target_link_libraries(${target} PRIVATE SDL2::SDL2main)
    endif()
endfunction()