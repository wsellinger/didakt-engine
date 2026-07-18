#include "SDLLoggingProvider.h"

#include "../../core/LogLevel.h"

#include <SDL_log.h>

void SDLLoggingProvider::Log(LogLevel level, const std::string& message)
{
	switch (level)
	{
    case LogLevel::Trace:
        SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "%s", message.c_str());
        break;
    case LogLevel::Debug:
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s", message.c_str());
        break;
    case LogLevel::Info:
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", message.c_str());
        break;
    case LogLevel::Warning:
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", message.c_str());
        break;
    case LogLevel::Error:
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", message.c_str());
        break;
    case LogLevel::Critical:
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "%s", message.c_str());
        break;
	default:
		break;
	}
}
