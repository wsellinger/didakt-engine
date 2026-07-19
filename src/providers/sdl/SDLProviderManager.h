#pragma once

#include "../interfaces/IProviderManager.h"

#include "SDLAssetProvider.h"
#include "SDLInputProvider.h"
#include "SDLLoggingProvider.h"
#include "SDLRenderProvider.h"
#include "SDLSystemTimerProvider.h"
#include "SDLWindowProvider.h"

struct SDL_Window;
struct SDL_Renderer;

class SDLProviderManager : public IProviderManager
{
public:
    bool Initialize(Config config) override;
    void Shutdown() override;

    IAssetProvider& GetAssetProvider() override { return _assetProvider; }
    IInputProvider& GetInputProvider() override { return _inputProvider; }
    ILoggingProvider& GetLoggingProvider() override { return _loggingProvider; }
    IRenderProvider& GetRenderProvider() override { return _renderProvider; }
    ISystemTimerProvider& GetSystemTimerProvider() override { return _systemTimerProvider; }
    IWindowProvider& GetWindowProvider() override { return _windowProvider; }

private:
    SDLAssetProvider _assetProvider;
    SDLInputProvider _inputProvider;
    SDLLoggingProvider _loggingProvider;
    SDLRenderProvider _renderProvider;
    SDLSystemTimerProvider _systemTimerProvider;
    SDLWindowProvider _windowProvider;

    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
};