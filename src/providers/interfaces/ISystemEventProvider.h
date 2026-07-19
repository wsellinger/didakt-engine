#pragma once

class ISystemEventProvider
{
public:
    virtual ~ISystemEventProvider() = default;
    
    virtual void PollEvents() = 0;

    virtual bool QuitRequested() const = 0;
};