#pragma once

#include "../../core/Config.h"

#include "IAssetProvider.h"
#include "IInputProvider.h"
#include "ILoggingProvider.h"
#include "IRenderProvider.h"
#include "ISystemTimerProvider.h"
#include "IWindowProvider.h"

class IProviderManager
{
public:
    virtual ~IProviderManager() = default;

    virtual bool Initialize(Config config) = 0;
    virtual void Shutdown() = 0;

    virtual IAssetProvider& GetAssetProvider() = 0;
    virtual IInputProvider& GetInputProvider() = 0;
    virtual ILoggingProvider& GetLoggingProvider() = 0;
    virtual IRenderProvider& GetRenderProvider() = 0;
    virtual ISystemTimerProvider& GetSystemTimerProvider() = 0;
    virtual IWindowProvider& GetWindowProvider() = 0;
};