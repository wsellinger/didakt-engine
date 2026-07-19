#pragma once

#include "../interfaces/ISystemEventProvider.h"

class SDLSystemEventProvider : public ISystemEventProvider
{
public:
    virtual void PollEvents() override;
    virtual bool QuitRequested() const override;

private:
    bool _quitRequested = false;
};