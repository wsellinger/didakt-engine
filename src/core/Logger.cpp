#include "Logger.h"

#include <cstdarg>
#include <cstdio>
#include <string>

namespace
{
    ILoggingProvider* _provider = nullptr;
}

namespace Logger
{
    void SetProvider(ILoggingProvider& provider)
    {
        _provider = &provider;
    }

    void Log(LogLevel level, const char* format, ...)
    {
        if (!_provider)
            return;
        
        //Get Args
        va_list args;
        va_start(args, format);

        //Get Buffer Size
        va_list argsCopy;
        va_copy(argsCopy, args);
        int size = std::vsnprintf(nullptr, 0, format, argsCopy);
        va_end(argsCopy);

        if (size >= 0)
        {
            size_t sizeT = static_cast<size_t>(size);
            //Format Message
            std::string message(sizeT, '\0');
            std::vsnprintf(message.data(), sizeT + 1, format, args);
            
            //Log Message
            _provider->Log(level, message);
        }

        //Close Args
        va_end(args);
    }
}