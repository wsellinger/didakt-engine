#pragma once

#include "../providers/interfaces/ILoggingProvider.h"

namespace Logger
{
    void SetProvider(ILoggingProvider& provider);
    void Log(LogLevel level, const char* format, ...);
};