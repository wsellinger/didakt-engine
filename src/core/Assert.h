#pragma once

#if defined(DIDAKT_USE_SDL)
    #include <SDL_Assert.h>
    #define DIDAKT_ASSERT(expr) SDL_assert(expr)
#else
    #include <cassert>
    #define DIDAKT_ASSERT(expr) assert(expr)
#endif