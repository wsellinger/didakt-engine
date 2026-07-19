#include "ProviderManagerFactory.h"

#if defined(DIDAKT_USE_SDL)
    #include "sdl/SDLProviderManager.h"
#else
    #error "No provider manager implementation defined for this platform."
#endif

std::unique_ptr<IProviderManager> CreateProviderManager()
{
#if defined(DIDAKT_USE_SDL)
    return std::make_unique<SDLProviderManager>();
#else
    #error "No provider manager implementation defined for this platform."
#endif
}