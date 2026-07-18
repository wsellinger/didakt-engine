#pragma once

#include "../interfaces/ISystemTimerProvider.h"

class SDLSystemTimerProvider : public ISystemTimerProvider
{
public:
    uint64_t GetTime() const override;
    uint64_t GetFrequency() const override;
};