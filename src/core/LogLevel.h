#pragma once

#include <cstdint>

enum class LogLevel : std::uint8_t
{
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Critical
};