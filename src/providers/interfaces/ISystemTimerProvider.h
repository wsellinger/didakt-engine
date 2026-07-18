#pragma once

#include <cstdint>

class ISystemTimerProvider
{
public:
    virtual ~ISystemTimerProvider() = default;

    virtual uint64_t GetTime() const = 0;
    virtual uint64_t GetFrequency() const = 0;
};