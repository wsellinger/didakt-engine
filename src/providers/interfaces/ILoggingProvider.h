#pragma once

#include "../../core/LogLevel.h"

#include <string>

class ILoggingProvider
{
    public:
    virtual ~ILoggingProvider() = default;

    virtual void Log(LogLevel level, const std::string& message) = 0;
};