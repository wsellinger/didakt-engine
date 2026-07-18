#pragma once

#include "../interfaces/ILoggingProvider.h"

class SDLLoggingProvider : public ILoggingProvider
{
public:
    void Log(LogLevel level, const std::string& message) override;
};
